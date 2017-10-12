#include "stdafx.h"
#include "SoftwareSkinningScene_3.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "Components/Components.h"


SoftwareSkinningScene_3::SoftwareSkinningScene_3(): GameScene(L"SoftwareSkinningScene_3"), m_pBone0(nullptr), m_pBone1(nullptr), m_BoneRotation(0), m_RotationSign(1), m_pMeshDrawer(nullptr)
{
}


SoftwareSkinningScene_3::~SoftwareSkinningScene_3()
{
}

void SoftwareSkinningScene_3::Initialize(const GameContext& gameContext)
{
	auto colorMat = new ColorMaterial(true);
	gameContext.pMaterialManager->AddMaterial(colorMat, 15);

	auto root = new GameObject();

	m_pBone0 = new BoneObject(0, 15, 15.f);
	m_pBone1 = new BoneObject(1, 15, 15.f);

	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);

	root->GetTransform()->Rotate(0.f, -90.f, 0.f, true);
	AddChild(root);

	m_pBone0->CalculateBindPose();

	m_pMeshDrawer = new MeshDrawComponent(24);
	auto meshDraw = new GameObject();
	meshDraw->AddComponent(m_pMeshDrawer);
	AddChild(meshDraw);

	CreateMesh(15.f);
}

void SoftwareSkinningScene_3::Update(const GameContext& gameContext)
{
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45.f)
		m_RotationSign = 1;
	else if (m_RotationSign > 0 && m_BoneRotation >= 45.f)
		m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0.f, 0.f);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2, 0.f, 0.f);


	auto bindPose0 = m_pBone0->GetBindPose();
	auto bindPose1 = m_pBone1->GetBindPose();

	auto boneTransform0 = XMMatrixMultiply(XMLoadFloat4x4(&bindPose0), XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld()));
	auto boneTransform1 = XMMatrixMultiply(XMLoadFloat4x4(&bindPose1), XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld()));

	for (size_t i = 0; i < m_SkinnedVertices.size(); ++i)
	{
		if (i < 24)
		{
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, XMVector3Transform(XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position), m_SkinnedVertices[i].blendWeight0 * boneTransform0 + m_SkinnedVertices[i].blendWeight1 * boneTransform1));
		}
		else
		{
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, XMVector3Transform(XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position), m_SkinnedVertices[i].blendWeight0 * boneTransform0 + m_SkinnedVertices[i].blendWeight1 * boneTransform1));

		}
	}

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		m_pMeshDrawer->AddQuad(QuadPosNormCol(m_SkinnedVertices[i].TransformedVertex,
			m_SkinnedVertices[i + 1].TransformedVertex,
			m_SkinnedVertices[i + 2].TransformedVertex,
			m_SkinnedVertices[i + 3].TransformedVertex));
	}

	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_3::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SoftwareSkinningScene_3::CreateMesh(float lenght)
{
	auto pos = XMFLOAT3(lenght / 2, 0, 0);
	auto offset = XMFLOAT3(lenght / 2, 2.5f, 2.5f);
	auto col = XMFLOAT4(1, 0, 0, 0.5f);

	//*****
	//BOX1*
	//*****

	//FRONT
	auto norm = XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	//BACK
	norm = XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//TOP
	norm = XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	//BOTTOM
	norm = XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	//LEFT
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 1.f, 0.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 1.f, 0.f));
	//RIGHT
	norm = XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));

	//*****
	//BOX2*
	//*****

	col = XMFLOAT4(0, 1, 0, 0.5f);
	pos = XMFLOAT3(22.5f, 0, 0);

	//FRONT
	norm = XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BACK
	norm = XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	//TOP
	norm = XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BOTTOM
	norm = XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//LEFT
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//RIGHT
	norm = XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, offset.z + pos.z), norm, col, 0.f, 1.f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y +
		pos.y, -offset.z + pos.z), norm, col, 0.f, 1.f));
}
