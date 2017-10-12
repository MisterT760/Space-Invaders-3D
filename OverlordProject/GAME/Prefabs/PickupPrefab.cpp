#include "stdafx.h"
#include "PickupPrefab.h"
#include "Physx/PhysxManager.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Scenegraph/GameScene.h"
#include "BlockPrefab.h"
#include "PlayerPrefab.h"


PickupPrefab::PickupPrefab(XMFLOAT3 spawnPoint, PickupType type) : m_pSpawnPoint(spawnPoint), m_Type(type)
{
}


PickupPrefab::~PickupPrefab()
{
}

void PickupPrefab::Activate()
{
	if (m_Type == HEALTH)
	{
		auto player = GetScene()->GetChild<PlayerPrefab>();

		if (player->GetHealth() < 80)
		{
			player->SetHealth(player->GetHealth() + 20);
		}
		else if (player->GetHealth() >= 80)
		{
			player->SetHealth(100.f);
		}
	}
	else if (m_Type == SHIELD)
	{
		auto player = GetScene()->GetChild<PlayerPrefab>();

		if (player->GetShield() < 60)
		{
			player->SetShield(player->GetShield() + 40);
		}
		else if (player->GetShield() >= 60)
		{
			player->SetShield(100.f);
		}
	}
	else if (m_Type == REBUILD)
	{
		m_BlockRebuilt = true;
	}
	else if (m_Type == RAPIDFIRE)
	{
		auto player = GetScene()->GetChild<PlayerPrefab>();
		if (player->GetInterval() > 0.3f)
		{
			player->SetInterval(player->GetInterval() - 0.1f);
		}
	}
}

void PickupPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	SetTag(L"Pickup");

	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);

	//Rigidbody
	AddComponent(new RigidBodyComponent());
	GetComponent<RigidBodyComponent>()->SetKinematic(true);

	//Collider + Model
	PxConvexMesh* convexGeom = nullptr;
	ModelComponent* modelComponent = nullptr;

	switch (m_Type)
	{
	case HEALTH:
		convexGeom = ContentManager::Load<PxConvexMesh>(L"./GAME/Resources/Meshes/PickUp_Health.ovpc");
		modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PickUp_Health.ovm");
		modelComponent->SetMaterial(12);
		break;
	case SHIELD:
		convexGeom = ContentManager::Load<PxConvexMesh>(L"./GAME/Resources/Meshes/PickUp_Shield.ovpc");
		modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PickUp_Shield.ovm");
		modelComponent->SetMaterial(13);
		break;
	case REBUILD: 
		convexGeom = ContentManager::Load<PxConvexMesh>(L"./GAME/Resources/Meshes/PickUp_Rebuild.ovpc");
		modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PickUp_Rebuild.ovm");
		modelComponent->SetMaterial(14);
		break;
	case RAPIDFIRE: 
		convexGeom = ContentManager::Load<PxConvexMesh>(L"./GAME/Resources/Meshes/PickUp_RapidFire.ovpc");
		modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PickUp_RapidFire.ovm");
		modelComponent->SetMaterial(15);
		break;
	default: ;
	}

	shared_ptr<PxGeometry> geometry(new PxConvexMeshGeometry(convexGeom));
	auto colliderComponent = new ColliderComponent(geometry, *defaultMaterial);
	colliderComponent->EnableTrigger(true);
	AddComponent(colliderComponent);

	//ModelComponent
	AddComponent(modelComponent);
	
	SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);

		if (action == ENTER && other->GetTag() == L"Player")
		{
			Activate();
			m_Active = false;
		}
	});
}

void PickupPrefab::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetTransform()->Translate(m_pSpawnPoint);
	GetTransform()->Scale(5.f, 5.f, 5.f);
	GetTransform()->Rotate(-90, 0, 0, true);
}

void PickupPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PickupPrefab::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PickupPrefab::Update(const GameContext& gameContext)
{
	GetTransform()->Rotate(-90, 0, m_Rotation);
	m_Rotation += gameContext.pGameTime->GetElapsed() * m_Speed;

	if (m_Timer > m_LifeTime)
	{
		m_Active = false;
	}

	m_Timer += gameContext.pGameTime->GetElapsed();
}
