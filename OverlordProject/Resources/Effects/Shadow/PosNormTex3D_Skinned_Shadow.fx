//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.115
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------

float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gLightViewProj;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];
float gShadowMapBias = 0.001f;

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerComparisonState samComp
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 indices : BLENDINDICES;
	float4 weights : BLENDWEIGHTS;
};

struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {

	VS_OUTPUT output;

	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;

	for (int i = 0; i < 4; i++)
	{
		if (input.indices[i] > -1.0f)
		{
			transformedPosition += mul(float4(input.pos, 1.0f), gBones[input.indices[i]]) * input.weights[i];
			transformedNormal += normalize(mul(input.normal, (float3x3)gBones[input.indices[i]])) * input.weights[i];
		}
	}
	transformedPosition.w = 1.0f;

	//Don't forget to change the output.pos & output.normal variables...
	output.pos = mul(transformedPosition, gWorldViewProj);
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld));

	output.texCoord = input.texCoord;
	output.lPos = mul(float4(input.pos, 1.0f), mul(gWorld, gLightViewProj));

	return output;
}

float2 textureOffset(int x, int y)
{
	return float2(x  * (1.0f / 1280.0f), y * (1.0f / 720.0f));
}

float EvaluateShadowMap(float4 lPos)
{
	//Re-homogenize position after interpolation
	lPos.xyz /= lPos.w;

	//If position is not visible to the light - dont illuminate it
	//Results in hard light frustrum
	if (lPos.x < -1.0f || lPos.x > 1.0f ||
		lPos.y < -1.0f || lPos.y > 1.0f ||
		lPos.z < 0.0f || lPos.z > 1.0f) return 1.0f;

	//Transform clip space coords to texture space coords
	lPos.xyz /= lPos.w;
	lPos.x = lPos.x / 2.0f + 0.5f;
	lPos.y = lPos.y / -2.0f + 0.5f;

	//Apply shadow map bias
	lPos.z -= gShadowMapBias;

	//PCF sampling for shadow map
	float sum = 0;
	float x, y;

	//Perform PCF filterin on a 4x4 texel neighborhood
	for (y = -2.0f; y <= 2.0f; y += 1.0f)
	{
		for (x = -2.0f; x < 2.0f; x += 1.0f)
		{
			sum += gShadowMap.SampleCmpLevelZero(samComp, lPos.xy + textureOffset(x, y), lPos.z);
		}
	}

	float shadowFactor = sum / 16.0f;

	return (shadowFactor * 0.5f) + 0.5f;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4(color_rgb * shadowValue, color_a);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

