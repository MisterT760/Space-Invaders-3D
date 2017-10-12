float4x4 gWorld : WORLD;
float4x4 gLightViewProj : LIGHT;
float4x4 gBones[70];
bool gHasBones = false;

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	FillMode = SOLID;
	CullMode = NONE;
};

struct VS_INPUT {
	float3 Position : POSITION;
	float4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT {
	float4 Position : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT ShadowMapVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//Skinning
	float4 originalPosition = float4(input.Position, 1);
	float4 transformedPosition = 0;

	if (gHasBones)
	{
		for (int i = 0; i < 4; ++i)
		{
			int boneIdx = input.blendIndices[i];
			if (boneIdx > -1)
			{
				transformedPosition += input.blendWeights[i] * mul(originalPosition, gBones[boneIdx]);
				transformedPosition.w = 1;
			}
		}
		output.Position = mul(transformedPosition, mul(gWorld, gLightViewProj));
		return output;
	}

	//No skinning
	output.Position = mul(originalPosition, mul(gWorld, gLightViewProj));
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void ShadowMapPS_VOID(float4 position:SV_POSITION) {}

float4 ShadowMapPS(float4 position:SV_POSITION) :SV_TARGET
{
	return float4(0.0f,1.0f,0.0f,1.0f);
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
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}