#include "stdafx.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Explode.h"
#include "EnemyLaserPrefab.h"
#include "Physx/PhysxManager.h"
#include "Components/Components.h"
#include "BaseEnemy.h"
#include "BlockPrefab.h"
#include "PlayerPrefab.h"


EnemyLaserPrefab::EnemyLaserPrefab(const XMFLOAT3 &direction, const float &speed, const int &matID) : m_pRigid(nullptr), m_pExplodeMat(nullptr), m_Direction(direction), m_Speed(speed), m_ExplodeTimer(0.f), m_Active(true), m_Explode(false), m_MatID(matID)
{
}


EnemyLaserPrefab::~EnemyLaserPrefab()
{
}

void EnemyLaserPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto defaultMaterial = physX->createMaterial(0.f, 0.f, 0.1f);
	
	//Rigidbody
	m_pRigid = new RigidBodyComponent();
	AddComponent(m_pRigid);

	//Mesh
	auto modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/Laser.ovm");
	modelComponent->SetMaterial(17);
	AddComponent(modelComponent);

	modelComponent->GetTransform()->Scale(0.5f, 0.5f, 0.15f);

	//Explode
	m_pExplodeMat = reinterpret_cast<Voxelizer_Explode*>(gameContext.pMaterialManager->GetMaterial(m_MatID));

	//Collider
	shared_ptr<PxGeometry> rectGeom(new PxBoxGeometry(0.5f, 0.5f, 4.f));
	AddComponent(new ColliderComponent(rectGeom, *defaultMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	GetComponent<ColliderComponent>()->EnableTrigger(true);
	
	SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);

		if (action == ENTER && other->GetTag() == L"FrontStop")
		{
			m_Active = false;
		}

		if (action == ENTER && other->GetTag() == L"Block")
		{
			reinterpret_cast<BlockPrefab*>(other)->Damage();
			m_Explode = true;
			GetComponent<ModelComponent>()->SetMaterial(m_MatID);
		}

		if (action == ENTER && other->GetTag() == L"Player")
		{
			reinterpret_cast<PlayerPrefab*>(other)->Damage(10.f);
			m_Explode = true;
			GetComponent<ModelComponent>()->SetMaterial(m_MatID);
		}
	});
}

void EnemyLaserPrefab::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void EnemyLaserPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void EnemyLaserPrefab::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void EnemyLaserPrefab::Update(const GameContext& gameContext)
{
	if (m_Explode)
	{
		m_ExplodeTimer += gameContext.pGameTime->GetElapsed();
		m_pExplodeMat->SetTime(m_ExplodeTimer);

		GetComponent<RigidBodyComponent>()->SetKinematic(true);

		if (m_ExplodeTimer > 0.25f)
		{
			m_Active = false;
		}
		return;
	}

	if (!m_Active)
	{
		return;
	}

	auto direction = XMLoadFloat3(&m_Direction);
	direction = XMVector3Normalize(direction);

	auto velocity = direction * m_Speed * gameContext.pGameTime->GetElapsed();
	XMFLOAT3 totalVel;
	XMStoreFloat3(&totalVel, velocity);

	m_pRigid->AddForce(ToPxVec3(totalVel), PxForceMode::eVELOCITY_CHANGE, true);
}
