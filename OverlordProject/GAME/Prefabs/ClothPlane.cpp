//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "ClothPlane.h"

// BASICS
// Managers
#include "Content\ContentManager.h"

// PhysX
#include "Physx\PhysxManager.h"
#include "Physx\PhysxProxy.h"

// General
#include "Components\Components.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"

// Specific
#include "SceneGraph\GameScene.h"
#include "Helpers\PhysxHelper.h"

ClothPlane::ClothPlane(const UINT columns, const UINT rows, const float width, const float depth, const UINT materialID, const XMFLOAT3 initPos, const vector<UINT> &fixedVerts)
	: GameObject()
	, m_Columns(columns)
	, m_Rows(rows)
	, m_Width(width)
	, m_Depth(depth)
	, m_InitialPose(initPos)
	, m_MaterialID(materialID)
	, m_NrOfVertices((rows + 1) * (columns + 1))
	, m_NrOfIndices(0)
	, m_FixedVerts(fixedVerts)
	, m_pMeshDrawer(nullptr)
	, m_pCloth(nullptr)
{
}


ClothPlane::~ClothPlane()
{
}

void ClothPlane::Initialize(const GameContext & gameContext)
{
	// MESH
	//******
	CreateMeshDrawer(gameContext);
	GetTransform()->Translate(m_InitialPose);

	// CLOTH
	//*******
	CreateCloth();
	m_pCloth->setGlobalPose(PxTransform(ToPxVec3(m_InitialPose)));
}
void ClothPlane::Update(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Set position according to transform
	m_pCloth->setTargetPose(PxTransform(ToPxVec3(GetTransform()->GetWorldPosition())));

	//FillMeshDrawer(gameContext, m_pMeshDrawer, m_pCloth);

	if (m_pMeshDrawer->GetVertexCapacity() != m_pCloth->getNbParticles())
	{
		Logger::LogWarning(L"ClothPlane::FillMeshDrawer() -> Trying to fill MeshDrawComponent with vertex capacity of: " + to_wstring(m_pMeshDrawer->GetVertexCapacity()) + L" with buffer of size: " + to_wstring(m_pCloth->getNbParticles()) + L".");
		return;
	}

	PxClothParticleData* data = m_pCloth->lockParticleData();

	if (data)
	{
		for (size_t i = 0; i < m_NrOfVertices; ++i)
		{
			m_pMeshDrawer->GetVertex(i).Position = ToXMFLOAT3(data->particles[i].pos);
		}
		m_pMeshDrawer->GenerateNormals();
		m_pMeshDrawer->UpdateVertexBuffer();

		data->unlock();
	}

}


void ClothPlane::FillMeshDrawer(const GameContext & gameContext, MeshIndexedDrawComponent* meshDrawer, PxCloth* pCloth)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (meshDrawer->GetVertexCapacity() != pCloth->getNbParticles())
	{
		Logger::LogWarning(L"ClothPlane::FillMeshDrawer() -> Trying to fill MeshDrawComponent with vertex capacity of: " + to_wstring(meshDrawer->GetVertexCapacity()) + L" with buffer of size: " + to_wstring(m_pCloth->getNbParticles()) + L".");
		return;
	}

	PxClothParticleData* data = pCloth->lockParticleData();

	if (data)
	{
		for (size_t i = 0; i < m_NrOfVertices; ++i)
		{
			meshDrawer->GetVertex(i).Position = ToXMFLOAT3(data->particles[i].pos);
		}
		meshDrawer->GenerateNormals();
		meshDrawer->UpdateVertexBuffer();

		data->unlock();
	}
}

void ClothPlane::CreateMeshDrawer(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// CREATE MESH
	//*************
	m_NrOfIndices = m_Columns * m_Rows * 6;
	m_pMeshDrawer = new MeshIndexedDrawComponent(m_NrOfVertices, m_NrOfIndices);


	// VERTEX BUFFER
	//***************
	// Precalculations
	float cellWidth = m_Width / m_Columns;
	float cellDepth = m_Depth / m_Rows;
	float cellXPos = -m_Width / 2;
	float cellZPos = m_Depth / 2;
	for (UINT r = 0; r <= m_Rows; ++r)
	{
		cellXPos = -m_Width / 2;
		for (UINT c = 0; c <= m_Columns; ++c)
		{
			// Position
			XMFLOAT3 pos = XMFLOAT3(cellXPos, 0, cellZPos);

			// UV
			XMFLOAT2 uv = XMFLOAT2(pos.x / m_Width, pos.z / m_Width);

			// Add vertex
			m_pMeshDrawer->AddVertex(VertexPosNormCol(pos, XMFLOAT3(0, 1, 0), XMFLOAT4(1, 1, 1, 1)));
			cellXPos += cellWidth;
		}
		cellZPos -= cellDepth;
	}

	// INDEX BUFFER
	//**************
	for (UINT row = 0; row < m_Rows; ++row)
	{
		for (UINT col = 0; col < m_Columns; ++col)
		{
			UINT a = row * (m_Columns + 1) + col;
			UINT b = a + 1;
			UINT c = a + m_Columns + 1;
			UINT d = c + 1;

			AddQuad(m_pMeshDrawer, a, b, c, d);
		}	
	}

	AddComponent(m_pMeshDrawer);
}
void ClothPlane::AddQuad(MeshIndexedDrawComponent* mesh, const UINT a, const UINT b, const UINT c, const UINT d)
{
	//Triangle 1
	mesh->AddIndex(a);
	mesh->AddIndex(c);
	mesh->AddIndex(d);

	//Triangle 2
	mesh->AddIndex(a);
	mesh->AddIndex(d);
	mesh->AddIndex(b);
}
void ClothPlane::AddAdjacencyQuad(MeshIndexedDrawComponent* mesh, const UINT a, const UINT b, const UINT c, const UINT d, const UINT ae, const UINT be, const UINT ce, const UINT de)
{
	//Triangle 1
	mesh->AddIndex(a);
	mesh->AddIndex(de);
	mesh->AddIndex(c);
	mesh->AddIndex(ce);
	mesh->AddIndex(d);
	mesh->AddIndex(b); 
	//Triangle 2
	mesh->AddIndex(a);
	mesh->AddIndex(c); 
	mesh->AddIndex(d);
	mesh->AddIndex(be);
	mesh->AddIndex(b);
	mesh->AddIndex(ae);
}

void ClothPlane::CreateCloth()
{
	// PhysX
	//*******
	PxPhysics *physX = PhysxManager::GetInstance()->GetPhysics();

	// BUFFERS
	//*********
	// Copy over vertex buffer
	PxClothParticle* vertices = new PxClothParticle[m_NrOfVertices];
	for (size_t i = 0; i < m_NrOfVertices; ++i)
	{
		XMFLOAT3 pos = m_pMeshDrawer->GetVertex(i).Position;
		float invWeight = 1.0f;
		if (find(m_FixedVerts.begin(), m_FixedVerts.end(), i) != m_FixedVerts.end())
			invWeight = 0.0f;
		vertices[i] = PxClothParticle(ToPxVec3(pos), invWeight);
	}

	// Create primitive buffer
	PxU32* quads = new PxU32[m_Columns * m_Rows * 4];
	for (UINT row = 0; row < m_Rows; ++row)
	{
		for (UINT col = 0; col < m_Columns; ++col)
		{
			UINT i = (row * m_Columns + col) * 4;
			UINT a = row * (m_Columns + 1) + col;
			UINT b = a + 1;
			UINT d = a + m_Columns + 1;
			UINT c = d + 1;
			quads[i + 0] = a;
			quads[i + 1] = b;
			quads[i + 2] = c;
			quads[i + 3] = d;
		}
	}

	// CLOTH
	//*******
	// Mesh descriptor
	PxClothMeshDesc meshDesc;
	meshDesc.points.data = vertices;
	meshDesc.points.count = m_NrOfVertices;
	meshDesc.points.stride = sizeof(PxClothParticle);

	meshDesc.invMasses.data = &vertices->invWeight;
	meshDesc.invMasses.count = m_NrOfVertices;
	meshDesc.invMasses.stride = sizeof(PxClothParticle);

	meshDesc.quads.data = quads;
	meshDesc.quads.count = m_Columns * m_Rows;
	meshDesc.quads.stride = sizeof(PxU32) * 4;

	// Validity check
	if (!(meshDesc.isValid()))
		Logger::LogError(L"ClothPlane::CreateCloth() -> Invalid mesh descriptor");

	// Create fabric
	PxClothFabric* fabric = PxClothFabricCreate(*physX, meshDesc, PxVec3(0, 1, 0));

	// Create cloth
	PxTransform transform = PxTransform::createIdentity();
	PxClothFlags flag = PxClothFlag::eSCENE_COLLISION;
	m_pCloth = physX->createCloth(transform, *fabric, vertices, PxClothFlag::eSCENE_COLLISION);
	m_pCloth->setSolverFrequency(100.f);
	m_pCloth->userData = this;
	m_pCloth->setGlobalPose(PxTransform(ToPxVec3(GetTransform()->GetWorldPosition())));
	m_pCloth->setFrictionCoefficient(0.5);


	// Add cloth to scene
	GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*m_pCloth);

	// Delete dynamic arrays
	delete[] vertices;
	delete[] quads;
}