//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;
Texture2D gNoiseTexture;

float3 gDistortionOffsets;
float gDistortionTime;
float gDistortionFrequency;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState DepthEnable
{
	DepthEnable = TRUE;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState Back_Culling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	// Set the TexCoord
	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord;
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	float distortion = gNoiseTexture.Sample(samPoint, (input.TexCoord.y + gDistortionTime) * gDistortionFrequency, 0.5f);
	float3 offsets = distortion * gDistortionOffsets;
	float3 color_rgb = float3(gTexture.Sample(samPoint, float2(input.TexCoord.x + offsets.r, input.TexCoord.y)).r,
							  gTexture.Sample(samPoint, float2(input.TexCoord.x + offsets.g, input.TexCoord.y)).g,
							  gTexture.Sample(samPoint, float2(input.TexCoord.x + offsets.b, input.TexCoord.y)).b);

	return float4(color_rgb, 1.f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
		SetRasterizerState(Back_Culling);
		SetDepthStencilState(DepthEnable, 0);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}