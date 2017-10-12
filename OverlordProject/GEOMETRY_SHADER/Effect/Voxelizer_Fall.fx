//************
// VARIABLES *
//************
float4x4 gMatrixViewInverse : VIEWINVERSE;

//Textures
Texture2D gNoiseTexture;
Texture2D gDiffuseTexture;

//Colors
float4 gColor : COLOR;

//Voxelizer
float gCubeSize = 0.1;

//Explosion
float gGravity = 9.18f;

//Timers
float gTimer : TIME;
float gSlowMotionFactor = 1.0f;
float gMaxFallTime = 1.0f;

cbuffer cbPerObject
{
	float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
	float4x4 gMatrixWorld : WORLD;
	float3 gLightDir={0.2f,-1.0f,0.2f};
}

RasterizerState NoCulling 
{ 
	CullMode = NONE; 
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// of Mirror of Clamp of Border
    AddressV = Wrap;// of Mirror of Clamp of Border
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	//Step 1.
	//Delete this transformation code and just return the VS_DATA parameter (vsData)
	//Don't forget to change the return type!
	return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal, float2 texCoord)
{
	GS_DATA gsData = (GS_DATA)0;
	gsData.Color = gColor;
	gsData.Position = mul(float4(pos, 1), gMatrixWorldViewProj);
	gsData.Normal = normal * gMatrixWorld[2];
	gsData.TexCoord = texCoord;
	triStream.Append(gsData);
}

float3 Fall(float3 basePoint, float3 normal)
{
	float time = gTimer / gSlowMotionFactor;
	if(gTimer > gMaxFallTime)
	{
		time = gMaxFallTime;
	}
	
	float gravity = gGravity * gNoiseTexture.SampleLevel(LinearSampler, normal, 1.0f);
	float3 fallPosition = basePoint;
	
	float fallSpeed = 0.5 * gravity * pow(time, 2.0);
	fallPosition.y -= fallSpeed;
	return fallPosition;
}

void Voxelize(VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	float3 basePoint, leftBotFront, rightBotFront, leftTopFront, rightTopFront, cubeNormal;
	float3 leftBotBack, rightBotBack, leftTopBack, rightTopBack;
	
	//calculate Basepoint
	basePoint = (vertices[0].Position + vertices[1].Position + vertices[2].Position) / 3;
	
	//calculate cubeNormal
	cubeNormal = (vertices[0].Normal + vertices[1].Normal + vertices[2].Normal) / 3;
		
	//Explode
	basePoint = Fall(basePoint, cubeNormal);
	
	//calculate squarePositions
	//Front
	leftBotFront = float3(basePoint.x - gCubeSize, basePoint.y + gCubeSize, basePoint.z + gCubeSize);
	rightBotFront = float3(basePoint.x + gCubeSize, basePoint.y + gCubeSize, basePoint.z + gCubeSize);
	leftTopFront = float3(basePoint.x - gCubeSize, basePoint.y - gCubeSize, basePoint.z + gCubeSize);
	rightTopFront = float3(basePoint.x + gCubeSize, basePoint.y - gCubeSize, basePoint.z + gCubeSize);
	
	//Back
	leftBotBack = float3(basePoint.x - gCubeSize, basePoint.y + gCubeSize, basePoint.z - gCubeSize);
	rightBotBack = float3(basePoint.x + gCubeSize, basePoint.y + gCubeSize, basePoint.z - gCubeSize);
	leftTopBack = float3(basePoint.x - gCubeSize, basePoint.y - gCubeSize, basePoint.z - gCubeSize);
	rightTopBack = float3(basePoint.x + gCubeSize, basePoint.y - gCubeSize, basePoint.z - gCubeSize);
	
	//Create Cube!
	//Front
	CreateVertex(triStream, leftTopFront, cubeNormal, float2(0,0));
	CreateVertex(triStream, rightTopFront, cubeNormal, float2(1,0));
	CreateVertex(triStream, leftBotFront, cubeNormal, float2(0,1));
	CreateVertex(triStream, rightBotFront, cubeNormal, float2(1,1));
	triStream.RestartStrip();
	
	//Back
	CreateVertex(triStream, leftTopBack, cubeNormal, float2(0,0));
	CreateVertex(triStream, rightTopBack, cubeNormal, float2(1,0));
	CreateVertex(triStream, leftBotBack, cubeNormal, float2(0,1));
	CreateVertex(triStream, rightBotBack, cubeNormal, float2(1,1));
	triStream.RestartStrip();
	
	//Left
	CreateVertex(triStream, leftTopFront, cubeNormal, float2(0,0));
	CreateVertex(triStream, leftTopBack, cubeNormal, float2(1,0));
	CreateVertex(triStream, leftBotFront, cubeNormal, float2(0,1));
	CreateVertex(triStream, leftBotBack, cubeNormal, float2(1,1));
	triStream.RestartStrip();
	
	//Right
	CreateVertex(triStream, rightTopFront, cubeNormal, float2(0,0));
	CreateVertex(triStream, rightTopBack, cubeNormal, float2(1,0));
	CreateVertex(triStream, rightBotFront, cubeNormal, float2(0,1));
	CreateVertex(triStream, rightBotBack, cubeNormal, float2(1,1));
	triStream.RestartStrip();
	
	//Top
	CreateVertex(triStream, leftTopFront, cubeNormal, float2(0,0));
	CreateVertex(triStream, rightTopFront, cubeNormal, float2(1,0));
	CreateVertex(triStream, leftTopBack, cubeNormal, float2(0,1));
	CreateVertex(triStream, rightTopBack, cubeNormal, float2(1,1));
	triStream.RestartStrip();
	
	//Bot
	CreateVertex(triStream, leftBotFront,  cubeNormal, float2(0,0));
	CreateVertex(triStream, rightBotFront,cubeNormal, float2(1,0));
	CreateVertex(triStream, leftBotBack,  cubeNormal, float2(0,1));
	CreateVertex(triStream, rightBotBack, cubeNormal, float2(1,1));
	triStream.RestartStrip();
}

[maxvertexcount(24)]
void MainGS(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{	
		Voxelize(vertices, triStream);
}                                        

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET 
{
	float4 diffuseColor = gDiffuseTexture.Sample(LinearSampler, input.TexCoord);
	diffuseColor *= gColor * 5;
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a - (gTimer / gMaxFallTime);
	
	input.Normal=-normalize(input.Normal);
	float s = max(dot(gLightDir,input.Normal), 0.4f);
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.Normal, -gLightDir);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	
	return float4(color_rgb * s, color_a);
}


//*************
// TECHNIQUES *
//*************
technique10 DefaultTechnique 
{
	pass p0 {
		SetRasterizerState(NoCulling);	
		SetBlendState(EnableBlending, float4(1.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}