#include "stdafx.h"
#include "BaseEnemy.h"
#include "Physx/PhysxManager.h"
#include "Components/Components.h"


BaseEnemy::BaseEnemy(const XMFLOAT3 spawnPoint) : m_Health(100), m_Active(true), m_SpawnPoint(spawnPoint), m_Direction(0,0,0)
{
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Set Tag
	SetTag(L"Enemy");

	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);
	
	//Rigidbody
	AddComponent(new RigidBodyComponent());

	shared_ptr<PxGeometry> boxGeom(new PxBoxGeometry(3.f, 3.f, 3.f));
	AddComponent(new ColliderComponent(boxGeom, *defaultMaterial, PxTransform(PxIdentity)));
	
	GetTransform()->Translate(m_SpawnPoint);
}

void BaseEnemy::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void BaseEnemy::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BaseEnemy::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BaseEnemy::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BaseEnemy::Damage()
{
	m_Health -= 10;

	if (m_Health <= 0)
	{
		m_Active = false;
	}
}
