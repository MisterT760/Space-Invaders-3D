float4x4 gTransform : WORLDVIEWPROJECTION;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2; //Texture Channel
	float3 Position : POSITION; //Left-Top Character Quad Starting Position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Left-Top Character Texture Coordinate on Texture
	float2 CharSize: TEXCOORD1; //Size of the character (in screenspace)
};

struct GS_DATA
{
	float4 Position : SV_POSITION; //HOMOGENEOUS clipping space position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Texcoord of the vertex
	int Channel: TEXCOORD1; //Channel of the vertex
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	//Create a new GS_DATA object
	//Fill in all the fields
	//Append it to the TriangleStream

	GS_DATA gsData = (GS_DATA) 0;

	gsData.Position = mul(float4(pos, 1.0f), gTransform);
	gsData.Color = col;
	gsData.TexCoord = texCoord;
	gsData.Channel = channel;

	triStream.Append(gsData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Create a Quad using the character information of the given vertex
	//Note that the Vertex.CharSize is in screenspace, TextureCoordinates aren't ;) [Range 0 > 1]

	float3 leftTop = vertex[0].Position;
	float3 rightTop = float3(vertex[0].Position.x + vertex[0].CharSize.x, vertex[0].Position.y, vertex[0].Position.z);
	float3 leftBot = float3(vertex[0].Position.x, vertex[0].Position.y + vertex[0].CharSize.y, vertex[0].Position.z);
	float3 rightBot = float3(vertex[0].Position.x + vertex[0].CharSize.x, vertex[0].Position.y + vertex[0].CharSize.y, vertex[0].Position.z);

	float2 texCoordLT = vertex[0].TexCoord;
	float2 texCoordRT = float2(vertex[0].TexCoord.x + (vertex[0].CharSize.x / gTextureSize.x), vertex[0].TexCoord.y);
	float2 texCoordLB = float2(vertex[0].TexCoord.x, vertex[0].TexCoord.y + (vertex[0].CharSize.y / gTextureSize.y));
	float2 texCoordRB = float2(vertex[0].TexCoord.x + (vertex[0].CharSize.x / gTextureSize.x), vertex[0].TexCoord.y + (vertex[0].CharSize.y / gTextureSize.y));

	//1. Vertex Left-Top
	CreateVertex(triStream, leftTop, vertex[0].Color, texCoordLT, vertex[0].Channel);

	//2. Vertex Right-Top
	CreateVertex(triStream, rightTop, vertex[0].Color, texCoordRT, vertex[0].Channel);

	//3. Vertex Left-Bottom
	CreateVertex(triStream, leftBot, vertex[0].Color, texCoordLB, vertex[0].Channel);

	//4. Vertex Right-Bottom
	CreateVertex(triStream, rightBot, vertex[0].Color, texCoordRB, vertex[0].Channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {
	
	//Sample the texture and return the correct channel [Vertex.Channel]
	//You can iterate a float4 just like an array, using the index operator
	//Also, don't forget to colorize ;) [Vertex.Color]

	return gSpriteTexture.Sample(samPoint, input.TexCoord)[input.Channel] * input.Color;
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
