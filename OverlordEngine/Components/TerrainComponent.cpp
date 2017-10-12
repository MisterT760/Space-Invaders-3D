//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainComponent.h"

#include "..\Base\GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "TransformComponent.h"
#include "../Graphics/TextureData.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* TerrainComponent::m_pDiffuseSRVVariable = nullptr;
ID3DX11EffectShaderResourceVariable* TerrainComponent::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* TerrainComponent::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* TerrainComponent::m_pLightViewProjectionMatrix = nullptr;

TerrainComponent::TerrainComponent(const wstring& heightMapFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight) :
	m_HeightMapFile(heightMapFile),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows * nrOfColumns),
	m_NrOfTriangles(2 * (nrOfRows - 1) * (nrOfColumns - 1)),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr), m_DiffuseTexture(nullptr)
{
}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();

	m_pDiffuseSRVVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	m_pShadowSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
	m_pLightViewProjectionMatrix = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_DiffuseTexture = ContentManager::Load<TextureData>(L"./GAME/Resources/Textures/Terrain_Texture_Invert.png");

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	//CreatePxHeightField();

}

void TerrainComponent::Update(const GameContext& gameContext)
{ 
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainComponent::Draw(const GameContext& gameContext)
{ 
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));

	m_LightDirection = gameContext.pShadowMapper->GetLightDirection();

	m_pDiffuseSRVVariable->SetResource(m_DiffuseTexture->GetShaderResourceView());
	m_pLightDirectionVariable->SetRawValue(&m_LightDirection, 0, sizeof(m_LightDirection));
	m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowSRV());
	m_pLightViewProjectionMatrix->SetMatrix(reinterpret_cast<float*>(&gameContext.pShadowMapper->GetLightVP()));

	// Set vertex buffer
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p< techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	m_VecHeightValues.resize(m_NrOfVertices, 0);

	std::ifstream inFile;
	inFile.open(m_HeightMapFile.c_str(), std::ios_base::binary);
	if (!inFile)
	{
		Logger::LogFormat(LogLevel::Error, L"Loading Terrain \'%ls\' failed!", m_HeightMapFile.c_str());
	}

	inFile.read(reinterpret_cast<char*>(&m_VecHeightValues[0]), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	inFile.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	//**VERTICES
	//Reserve spots in the buffer
	m_VecVertices.resize(m_NrOfVertices);

	//Calculate the Initial Position (Terrain centered on the origin)
	//Reset the cellXPos Position for each Column

	//1. Position -- Partially Exercise - Heightmap --
	//3. TexCoord -- Exercise - UV --

	//Move the cellXPos Position (Down)
	//Move the cellZPos Position (Right)

	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float cellZPos = m_Depth / 2.f;

	for (int row = 0; row < int(m_NrOfRows); ++row)
	{
		float cellXPos = -m_Width / 2.f;
		for (int col = 0; col < int(m_NrOfColumns); ++col)
		{
			int vertexId = row * m_NrOfColumns + col;

			m_VecVertices[vertexId].Position.x = cellXPos;
			m_VecVertices[vertexId].Position.y = float(m_VecHeightValues[vertexId]) / USHRT_MAX * m_MaxHeight;
			m_VecVertices[vertexId].Position.z = cellZPos;

			m_VecVertices[vertexId].TexCoord.x = float(col) / (m_NrOfColumns - 1);
			m_VecVertices[vertexId].TexCoord.y = float(row) / (m_NrOfRows - 1);

			cellXPos += cellWidth;
		}

		cellZPos -= cellDepth;
	}

	int nrQuadsRow = m_NrOfRows - 1;
	int nrQuadsColumn = m_NrOfColumns - 1;

	for (int row = 0; row < nrQuadsRow; ++row)
	{
		for (int col = 0; col < nrQuadsColumn; ++col)
		{
			int a = row * m_NrOfColumns + col;
			int b = a + 1;
			int c = a + m_NrOfColumns;
			int d = c + 1;
			AddQuad(a, b, c, d);
		}
	}

	//2. Normal

	//Exercise - Normals
	//For each face...
	//Get the positions of 6 vertices
	//first triangle
	//second triangle

	//iterate through the vertices and calculate a normal for each one of them using the average of the 6 adjacent faces

	//from left front to right back
	//if col==0 is on left edge, there are 
	//no vertices on the left, fill in a illegal index

	//if col==m_NumColumns-1 is on right edge, there are 
	//no vertices on the right, fill in a illegal index

	//if index<0 or out of range: front or back edge 
	//it may not be used to calculate the average

	//calculate average by normalizing

	for (unsigned int i = 0; i < m_VecIndices.size(); i += 6)
	{
		//6 vertices
		XMVECTOR a, b, c, d, e, f;
		a = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i)).Position);
		b = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 1)).Position);
		c = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 2)).Position);
		d = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 3)).Position);
		e = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 4)).Position);
		f = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 5)).Position);

		//First Triangle
		XMVECTOR v1, v2, normal;
		v1 = a - c;
		v2 = a - b;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMFLOAT3 normalFloat;
		XMStoreFloat3(&normalFloat, normal);
		m_VecFaceNormals.push_back(normalFloat);

		//Second Triangle
		v1 = f - e;
		v2 = f - d;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMStoreFloat3(&normalFloat, normal);
		m_VecFaceNormals.push_back(normalFloat);
	}

	int numFacesPerRow = (m_NrOfRows - 1) * 2;
	int index[6];
	for (unsigned int row = 0; row < m_NrOfRows; row++)
	{		
		for (unsigned int col = 0; col < m_NrOfColumns; col++)
		{
			int centerIndex = numFacesPerRow * row + col * 2;
			index[0] = centerIndex - 1;
			index[1] = centerIndex;
			index[2] = centerIndex + 1;
			index[3] = centerIndex - numFacesPerRow - 2;
			index[4] = centerIndex - numFacesPerRow - 1;
			index[5] = centerIndex - numFacesPerRow;
			
			if (col == 0)
			{
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}

			if (col == m_NrOfColumns)
			{
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}

			XMVECTOR sum = XMVectorZero();
			for (int i = 0; i < 6; ++i)
			{
				if ((index[i] >= 0) && (index[i] < int(m_VecFaceNormals.size())))
				{
					sum += XMLoadFloat3(&m_VecFaceNormals.at(index[i]));
				}
			}
			sum = XMVector3Normalize(sum);

			int vertexId = row * m_NrOfColumns + col;
			XMStoreFloat3(&m_VecVertices[vertexId].Normal, sum);
		}
	}
}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d)
{
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

//Exercise - PhysX
void TerrainComponent::CreatePxHeightField()
{
	//Implement PhysX HeightField
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto terrainMat = physX->createMaterial(0.1f, 0.1f, 0.1f);

	//Create Actor
	PxTransform localPose;
	localPose.p = PxVec3(-(m_Width / 2.f), GetTransform()->GetPosition().y, (m_Depth / 2.f));
	localPose.q = PxQuat(PxPiDivTwo, PxVec3(0, 1, 0));

	auto actor = physX->createRigidStatic(localPose);

	auto samples = new PxU32[m_NrOfColumns * m_NrOfRows];

	for (PxU32 row = 0; row < m_NrOfColumns; row++)
	{
		for (PxU32 col = 0; col < m_NrOfRows; col++)
		{
			auto vertexId = row * m_NrOfColumns + col;
			samples[vertexId] = PxU32(float(m_VecHeightValues[vertexId]) / USHRT_MAX * m_MaxHeight);
		}
	}

	//Create Desc
	PxHeightFieldDesc heightfieldDesc;
	heightfieldDesc.format = PxHeightFieldFormat::eS16_TM;
	heightfieldDesc.nbColumns = m_NrOfColumns;
	heightfieldDesc.nbRows = m_NrOfRows;
	heightfieldDesc.samples.data = samples;
	heightfieldDesc.samples.stride = sizeof(PxU32); // 2x 8-bit material indices + 16-bit height
	heightfieldDesc.thickness = -10.0f; // user-specified heightfield thickness
	heightfieldDesc.flags = PxHeightFieldFlags();

	if (!heightfieldDesc.isValid())
	{
		Logger::LogError(L"[Terrain Component] HeightField Desc was invalid!");
	}

	//Create Normal Geometry
	PxHeightFieldGeometry geometry;
	geometry.columnScale = PxReal(m_Width / float(m_NrOfColumns));
	geometry.rowScale = PxReal(m_Depth / float(m_NrOfRows));
	geometry.heightScale = 1.f;
	geometry.heightField = physX->createHeightField(heightfieldDesc);

	if (!geometry.isValid())
	{
		Logger::LogError(L"[Terrain Component] HeightField Geometry was invalid!");
	}

	delete[] samples;

	PxShape* shape = actor->createShape(geometry, *terrainMat);
	shape->setQueryFilterData(PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0));
	shape->setSimulationFilterData(PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0));

	GetGameObject()->GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*actor);
	//After implementing the physX representation - Fix the lagginess from the PhysX Debugger!!!
}
