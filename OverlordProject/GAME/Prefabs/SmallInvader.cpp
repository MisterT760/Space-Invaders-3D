#include "stdafx.h"
#include "SmallInvader.h"
#include "Scenegraph/GameScene.h"
#include "Components/Components.h"
#include "Physx/PhysxManager.h"
#include "BlockPrefab.h"
#include "Components/EnemyController.h"
#include "Physx/PhysxProxy.h"
#include "EnemyLaserPrefab.h"
#include "Graphics/ModelAnimator.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Fall.h"


SmallInvader::SmallInvader(const XMFLOAT3 spawnPoint, const float x_Offset, const float z_Offset, const float speed, const int matID, const int laserMatID) : BaseEnemy(spawnPoint), m_X_BaseOffset(x_Offset), m_Z_BaseOffset(z_Offset), m_X_Offset(x_Offset), m_Z_Offset(z_Offset), m_Speed(speed), m_LaserMatID(laserMatID), m_MatID(matID), m_pFallMat(nullptr)
{
}


SmallInvader::~SmallInvader()
{
	RemoveComponent(m_pController);
	SafeDelete(m_pController);
}

XMFLOAT3 SmallInvader::GetControllerPosition() const
{
	return m_pController->GetPosition();
}

void SmallInvader::Damage()
{
	m_Health -= 10;

	if (m_Health <= 0)
	{
		m_Fall = true;
		m_pModelComponent->SetMaterial(m_MatID);
	}
}

void SmallInvader::Initialize(const GameContext& gameContext)
{
	//Set Tag
	SetTag(L"SmallInvader");

	//Set Health + StartTime
	m_Health = 100;
	m_StartTime = rand() % 5 + 1;

	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);

	//Add Controller
	m_pController = new EnemyController(defaultMaterial, XMFLOAT3(0, 1, 0), 5.f, L"SmallInvader");
	AddComponent(m_pController);

	//Add Model
	m_pModelComponent = new ModelComponent(L"./GAME/Resources/Meshes/SmallInvader.ovm");
	m_pModelComponent->SetMaterial(18);
	AddComponent(m_pModelComponent);

	m_pModelComponent->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	m_pModelComponent->GetTransform()->Rotate(180.f, 0.f, 180.f);

	if (m_pModelComponent->HasAnimator())
	{
		m_pModelComponent->GetAnimator()->SetAnimation(0);
		m_pModelComponent->GetAnimator()->Play();
	}

	//Fall Mat
	m_pFallMat = reinterpret_cast<Voxelizer_Fall*>(gameContext.pMaterialManager->GetMaterial(m_MatID));

	//Set Offsets
	m_X_Offset = m_X_BaseOffset;
	m_Z_Offset = m_Z_BaseOffset;

	//Start Position
	GetTransform()->Translate(m_SpawnPoint);

	//Should Animate
	auto anim = rand() % 4;

	if (anim == 0)
	{
		m_ShouldAnimate = true;
	}
}

void SmallInvader::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_pModelComponent->HasAnimator())
	{
		m_pModelComponent->GetAnimator()->SetAnimation(0);
	}
}

void SmallInvader::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SmallInvader::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SmallInvader::Update(const GameContext& gameContext)
{
	if (m_Fall)
	{
		m_FallTime += gameContext.pGameTime->GetElapsed();
		m_pFallMat->SetTime(m_FallTime);

		if (m_FallTime > 1.5f)
		{
			m_Active = false;
		}
	}
}

void SmallInvader::Movement(const GameContext& gameContext)
{
	if (m_Timer > m_StartTime)
	{
		if (m_ShouldAnimate)
		{
			m_pModelComponent->GetAnimator()->Play();
		}
	}

	auto direction = m_Direction;
	XMStoreFloat3(&direction, XMLoadFloat3(&m_Direction) * m_Speed * gameContext.pGameTime->GetElapsed());
	m_pController->Move(direction);

	if (m_X_Offset <= 0)
	{
		if (m_Z_Offset > 0)
		{
			m_Direction = { 0,0,-1 };
			m_Z_Offset -= m_Speed * gameContext.pGameTime->GetElapsed();
		}
		else
		{
			m_X_Direction *= -1;
			m_Direction = { 1 * m_X_Direction,0,0 };
			m_X_Offset = m_X_BaseOffset * 2;

			if (m_MoveZ)
			{
				m_Z_Offset = m_Z_BaseOffset;
			}
			else
				m_Z_Offset = 0.f;
		}
	}

	m_X_Offset -= m_Speed * gameContext.pGameTime->GetElapsed();
	m_pModelComponent->GetTransform()->Translate(m_pController->GetPosition());

	m_Timer += gameContext.pGameTime->GetElapsed();
}

EnemyLaserPrefab* SmallInvader::Attack(const GameContext& gameContext)
{
	if (m_Timer >= m_Interval)
	{
		m_Timer = 0;
		const PxU32 bufSize = 20;
		PxRaycastHit hit[bufSize];
		PxRaycastBuffer buf(hit, bufSize);

		if (GetScene()->GetPhysxProxy()->Raycast(ToPxVec3(m_pController->GetPosition()) + PxVec3(0,0, -6.5f), PxVec3(0, 0, -1), 5000, buf))
		{
			for (PxU32 i = 0; i < buf.nbTouches; i++)
			{
				BaseComponent* component = static_cast<BaseComponent*>(buf.touches[i].actor->userData);
				if (component)
				{
					GameObject* gameObj = component->GetGameObject();
					auto tag = gameObj->GetTag();

					if (tag == L"SmallInvader")
					{
						break;
					}

					if (tag == L"Player" || tag == L"Block")
					{
						auto laser = new EnemyLaserPrefab(XMFLOAT3(0, 0, -1), m_ShootSpeed, m_LaserMatID);
						laser->GetTransform()->Translate(m_pController->GetPosition());

						return laser;
					}
				}
			}
		}
	}

	m_Timer += gameContext.pGameTime->GetElapsed();
	return nullptr;
}
