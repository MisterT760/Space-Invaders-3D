#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "Components/Components.h"


SoftwareSkinningScene_1::SoftwareSkinningScene_1(): GameScene(L"SoftwareSkinningScene_1"), m_pBone0(nullptr), m_pBone1(nullptr), m_BoneRotation(0), m_RotationSign(1)
{
}


SoftwareSkinningScene_1::~SoftwareSkinningScene_1()
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	auto colorMat = new ColorMaterial(false);
	gameContext.pMaterialManager->AddMaterial(colorMat, 15);

	auto root = new GameObject();

	m_pBone0 = new BoneObject(0, 15, 15.f);
	m_pBone1 = new BoneObject(1, 15, 15.f);

	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);

	root->GetTransform()->Rotate(0.f, -90.f, 0.f, true);

	AddChild(root);
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45.f)
		m_RotationSign = 1;
	else if (m_RotationSign > 0 && m_BoneRotation >= 45.f)
		m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0.f, 0.f);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2, 0.f, 0.f);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
