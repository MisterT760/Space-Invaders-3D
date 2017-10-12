float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 

//Shadow
float4x4 gLightViewProj;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.001f;
Texture2D gShadowMap;

//Texture + Color
float4 gDiffuseColor : COLOR;
bool gInvert = false;

Texture2D gDiffuseMap;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
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

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

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
	lPos.x = lPos.x / 2.0f + 0.5f;
	lPos.y = lPos.y / -2.0f + 0.5f;

	//Apply shadow map bias
	lPos.z -= gShadowMapBias;

	//PCF sampling for shadow map
	float sum = 0;
	float x, y;

	//Perform PCF filterin on a 4x4 texel neighborhood
	for (y = -1.5f; y <= 1.5f; y += 1.0f)
	{
		for (x = -1.5f; x < 1.5f; x += 1.0f)
		{
			sum += gShadowMap.SampleCmpLevelZero(samComp, lPos.xy + textureOffset(x, y), lPos.z);
		}
	}

	float shadowFactor = sum / 16.0f;

	return (shadowFactor * 0.5f) + 0.5f;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	output.lPos = mul(float4(input.pos, 1.0f), mul(gWorld, gLightViewProj));
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	
	if (gInvert)
	{
		diffuseColor = float4(1.f - diffuseColor.rgb, 1.f);
	}
	
	diffuseColor *= gDiffuseColor * 5;
	float3 color_rgb = diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

