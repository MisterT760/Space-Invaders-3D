#include "stdafx.h"
#include "BlockPrefab.h"
#include "Components/Components.h"
#include "Physx/PhysxManager.h"
#include "SmallInvader.h"
#include "../OverlordProject/GAME/Resources/Materials/MeshMaterial.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Disappear.h"
#include "Content/ContentManager.h"

BlockPrefab::BlockPrefab(XMFLOAT3 spawnPoint, int materialID, int disappearMat) : m_Health(250), m_DisappearTimer(0), m_Active(true), m_Disappear(false), m_SpawnPoint(spawnPoint), m_MaterialID(materialID), m_DisappearMatID(disappearMat), m_pMat(nullptr), m_pDisappearMat(nullptr)
{
}


BlockPrefab::~BlockPrefab()
{
}

void BlockPrefab::Damage()
{
	m_Health -= 10 * 2.25;
	auto color = m_pMat->GetDiffuseColor();
	color.x = 1.0f - (m_Health / 250);
	color.y = m_Health / 250;

	m_pMat->SetDiffuseColor(color);

	if (m_Health <= 0)
	{
		m_Disappear = true;
		GetComponent<ModelComponent>()->SetMaterial(m_DisappearMatID);
	}
}

void BlockPrefab::Initialize(const GameContext& gameContext)
{
	//Set Tag
	SetTag(L"Block");

	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);

	//Model
	auto modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/Block.ovm");
	m_pMat = reinterpret_cast<MeshMaterial*>(gameContext.pMaterialManager->GetMaterial(m_MaterialID));
	m_pMat->SetDiffuseColor(XMFLOAT4(0.f, 1.0f, 0.f, 1.0f));
	modelComponent->SetMaterial(m_MaterialID);
	AddComponent(modelComponent);

	//Disappear
	m_pDisappearMat = reinterpret_cast<Voxelizer_Disappear*>(gameContext.pMaterialManager->GetMaterial(m_DisappearMatID));

	//Rigidbody
	AddComponent(new RigidBodyComponent());
	GetComponent<RigidBodyComponent>()->SetKinematic(true);

	//Collider
	shared_ptr<PxGeometry> boxGeom(new PxBoxGeometry(17.5f, 4.5f, 2.5f));
	auto colliderComponent = new ColliderComponent(boxGeom, *defaultMaterial);
	AddComponent(colliderComponent);

	GetTransform()->Translate(m_SpawnPoint);
}

void BlockPrefab::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	GetTransform()->Scale(0.2f, 0.2f, 0.2f);
}

void BlockPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BlockPrefab::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BlockPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_Disappear)
	{
		m_DisappearTimer += gameContext.pGameTime->GetElapsed();
		m_pDisappearMat->SetTime(m_DisappearTimer);

		if (m_DisappearTimer > 1.f)
		{
			m_Active = false;
			return;
		}
	}
}
