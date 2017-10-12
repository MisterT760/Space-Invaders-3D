#include "stdafx.h"
#include "BoneObject.h"
#include "Components/Components.h"


BoneObject::BoneObject(int boneId, int materialId, float lenght): m_Lenght(lenght), m_BoneId(boneId), m_MaterialId(materialId)
{
}


BoneObject::~BoneObject()
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(0.f, 0.f, -m_Lenght);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld())));
	
	auto childeren = GetChildren<BoneObject>();

	if (!childeren.empty())
	{
		for (size_t i = 0; i < childeren.size(); ++i)
		{
			childeren[i]->CalculateBindPose();
		}
	}
}

void BoneObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto empty = new GameObject();
	auto modelComp = new ModelComponent(L"./Resources/Meshes/Bone.ovm");
	modelComp->SetMaterial(m_MaterialId);
	empty->AddComponent(modelComp);

	AddChild(empty);

	empty->GetTransform()->Scale(m_Lenght, m_Lenght, m_Lenght);
}

