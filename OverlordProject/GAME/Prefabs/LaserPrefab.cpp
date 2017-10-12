#include "stdafx.h"
#include "LaserPrefab.h"
#include "Physx/PhysxManager.h"
#include "Components/Components.h"
#include "BaseEnemy.h"
#include "SmallInvader.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Explode.h"


LaserPrefab::LaserPrefab(const XMFLOAT3 &direction, const float &speed, const int &matID) : m_pRigid(nullptr), m_pExplodeMat(nullptr), m_Direction(direction), m_Speed(speed), m_ExplodeTimer(0), m_Active(true), m_Explode(false), m_MatID(matID)
{
}


LaserPrefab::~LaserPrefab()
{
}

void LaserPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto defaultMaterial = physX->createMaterial(0.f, 0.f, 0.1f);
	
	//Rigidbody
	m_pRigid = new RigidBodyComponent();
	AddComponent(m_pRigid);

	//Mesh
	auto modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/Laser.ovm");
	modelComponent->SetMaterial(16);
	AddComponent(modelComponent);

	modelComponent->GetTransform()->Scale(0.5f, 0.5f, 0.15f);

	shared_ptr<PxGeometry> rectGeom(new PxBoxGeometry(0.25f, 0.25f, 4.f));
	AddComponent(new ColliderComponent(rectGeom, *defaultMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	GetComponent<ColliderComponent>()->EnableTrigger(true);
	
	//Explode Material
	m_pExplodeMat = reinterpret_cast<Voxelizer_Explode*>(gameContext.pMaterialManager->GetMaterial(m_MatID));

	SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);

		if (action == ENTER && other->GetTag() == L"Backstop")
		{
			m_Active = false;
		}

		if (action == ENTER && other->GetTag() == L"SmallInvader")
		{
			reinterpret_cast<SmallInvader*>(other)->Damage();
			GetComponent<ModelComponent>()->SetMaterial(m_MatID);
			m_Explode = true;
		}

		if (action == ENTER && other->GetTag() == L"Block")
		{
			GetComponent<ModelComponent>()->SetMaterial(m_MatID);
			m_Explode = true;
		}
	});
}

void LaserPrefab::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void LaserPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void LaserPrefab::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void LaserPrefab::Update(const GameContext& gameContext)
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
