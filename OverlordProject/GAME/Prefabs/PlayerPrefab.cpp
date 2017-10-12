#include "stdafx.h"
#include "PlayerPrefab.h"
#include "LaserPrefab.h"
#include "Components/Components.h"
#include "Physx/PhysxManager.h"
#include "../OverlordProject/GAME/Resources/Materials/MeshMaterial.h"
#include "../OverlordProject/Materials/ColorMaterial.h"
#include "Components/ParticleEmitterComponent.h"


PlayerPrefab::PlayerPrefab(): m_pController(nullptr),
                              m_MaxVelocity(50.f),
                              m_AccelerationTime(0.3f),
                              m_Acceleration(m_MaxVelocity / m_AccelerationTime),
                              m_Velocity(0), m_pParticleLeft(nullptr), m_pParticleRight(nullptr)
{
}


PlayerPrefab::~PlayerPrefab()
{
}

void PlayerPrefab::Initialize(const GameContext& gameContext)
{
	//Set Tag
	SetTag(L"Player");

	//Controller
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto controllerMat = physX->createMaterial(0.f, 0.f, 0.1f);
	m_pController = new ControllerComponent(controllerMat, XMFLOAT3(0, 0, 1));
	AddComponent(m_pController);

	auto modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PlayerShip.ovm");
	m_pMat = new MeshMaterial();
	m_pMat->SetDiffuseColor(static_cast<XMFLOAT4>(Colors::Cyan));
	m_pMat->SetDiffuseTexture(L"./GAME/Resources/Textures/Player_Texture.png");
	m_pMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(m_pMat, 0);
	modelComponent->SetMaterial(0);
	AddComponent(modelComponent);
	modelComponent->GetTransform()->Scale(0.05f, 0.05f, 0.05f);
	modelComponent->GetTransform()->Rotate(0.f, -90.f, 0.f);

	//Particles
	m_pParticleLeft = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 50);
	m_pParticleLeft->SetVelocity(XMFLOAT3(0, 0, -7.5f));
	m_pParticleLeft->SetMinSize(1.0f);
	m_pParticleLeft->SetMaxSize(3.0f);
	m_pParticleLeft->SetMinEnergy(1.0f);
	m_pParticleLeft->SetMaxEnergy(2.0f);
	m_pParticleLeft->SetMinSizeGrow(3.5f);
	m_pParticleLeft->SetMaxSizeGrow(6.5f);
	m_pParticleLeft->SetMinEmitterRange(0.2f);
	m_pParticleLeft->SetMaxEmitterRange(0.75f);
	m_pParticleLeft->SetColor(XMFLOAT4(1.f, 1.f, 1.f, 0.6f));

	AddComponent(m_pParticleLeft);
	m_pParticleLeft->GetTransform()->Translate(-2, 0, -5);

	m_pParticleRight = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 50);
	m_pParticleRight->SetVelocity(XMFLOAT3(0, 0, -7.5f));
	m_pParticleRight->SetMinSize(1.0f);
	m_pParticleRight->SetMaxSize(3.0f);
	m_pParticleRight->SetMinEnergy(1.0f);
	m_pParticleRight->SetMaxEnergy(2.0f);
	m_pParticleRight->SetMinSizeGrow(3.5f);
	m_pParticleRight->SetMaxSizeGrow(6.5f);
	m_pParticleRight->SetMinEmitterRange(0.2f);
	m_pParticleRight->SetMaxEmitterRange(0.75f);
	m_pParticleRight->SetColor(XMFLOAT4(1.f, 1.f, 1.f, 0.6f));

	AddComponent(m_pParticleRight);
	m_pParticleRight->GetTransform()->Translate(2, 0, -5);
	

	//Input Actions (TODO: ADD CONTROLLER INPUT)
	gameContext.pInput->AddInputAction(InputAction(LEFT, Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(RIGHT, Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(FORWARD, Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(BACKWARD, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(SHOOTPULSE, Down, 'E', -1, XINPUT_GAMEPAD_A, PlayerOne));
	//ADD EXTRA IF NECESSARY
}

void PlayerPrefab::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PlayerPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PlayerPrefab::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PlayerPrefab::Update(const GameContext& gameContext)
{
	if (m_IsDead)
	{
		return;
	}

	//MOVEMENT
	XMFLOAT3 direction = XMFLOAT3(0, 0, 0);
	direction.z = gameContext.pInput->IsActionTriggered(FORWARD) ? 1.0f : 0.0f;
	if (direction.z == 0) direction.z = -(gameContext.pInput->IsActionTriggered(BACKWARD) ? 1.0f : 0.0f);
	
	direction.x = gameContext.pInput->IsActionTriggered(RIGHT) ? 1.0f : 0.0f;
	if (direction.x == 0) direction.x = -(gameContext.pInput->IsActionTriggered(LEFT) ? 1.0f : 0.0f);

	if (gameContext.pInput->IsGamepadConnected(GamepadIndex::PlayerOne))
	{
		direction.z = gameContext.pInput->GetThumbstickPosition(true, PlayerOne).y;
		direction.x = gameContext.pInput->GetThumbstickPosition(true, PlayerOne).x;
	}

	//VELOCITY
	if (m_Velocity < m_MaxVelocity) m_Velocity += m_Acceleration * gameContext.pGameTime->GetElapsed();
	else m_Velocity = m_MaxVelocity;

	XMFLOAT3 totalMovement = XMFLOAT3(0, 0, 0);
	XMStoreFloat3(&totalMovement, (XMLoadFloat3(&direction) * m_Velocity * gameContext.pGameTime->GetElapsed()));

	m_pController->Move(totalMovement);

	m_Timer += gameContext.pGameTime->GetElapsed();
}

LaserPrefab* PlayerPrefab::ShootLeft(const GameContext& gameContext, const XMFLOAT3& direction, const float& speed)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_IsDead)
	{
		return nullptr;
	}

	if (m_Timer >= m_Interval)
	{
		auto playerPos = GetTransform()->GetWorldPosition();
		playerPos.x -= 3.5f;
		playerPos.z += 3;
		auto laser = new LaserPrefab(direction, speed, 50);
		laser->GetTransform()->Translate(playerPos);
		return laser;
	}

	return nullptr;
}

LaserPrefab* PlayerPrefab::ShootRight(const GameContext& gameContext, const XMFLOAT3& direction, const float& speed)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_IsDead)
	{
		return nullptr;
	}

	if (m_Timer >= m_Interval)
	{
		auto playerPos = GetTransform()->GetWorldPosition();
		playerPos.x += 3.5f;
		playerPos.z += 3;
		auto laser = new LaserPrefab(direction, speed, 51);
		laser->GetTransform()->Translate(playerPos);
		return laser;
	}

	return nullptr;
}

void PlayerPrefab::Damage(float damage)
{
	if(m_Shield > 0)
	{
		if (m_Shield < damage)
		{
			m_Shield = 0;
		}
		else
		{
			m_Shield -= damage * 2.25f;
			if (m_Shield < 0)
			{
				m_Shield = 0;
			}
		}
	}
	else
	{
		m_Health -= damage * 3.f;

		if (m_Health <= 0)
		{
			m_Health = 0;
			m_IsDead = true;
		}
	}
}
