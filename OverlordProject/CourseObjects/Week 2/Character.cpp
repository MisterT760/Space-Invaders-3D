//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Character.h"
#include "Scenegraph\GameScene.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"

using namespace DirectX;

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Create controller
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto controlMaterial = physX->createMaterial(0.f, 0.f, 0.1f);
	m_pController = new ControllerComponent(controlMaterial);
	AddComponent(m_pController);

	// Add a fixed camera as child
	auto camera = new GameObject();
	m_pCamera = new CameraComponent();
	camera->AddComponent(m_pCamera);
	AddChild(camera);
	camera->GetTransform()->Translate(0.f, 2.5f, -17.5f);

	// Register all Input Actions
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::JUMP, InputTriggerState::Pressed, VK_SPACE));
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	m_pCamera->SetActive();
}


void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Update the character (Camera rotation, Character Movement, Character Gravity)
	//CAMERA
	XMFLOAT2 look = XMFLOAT2(0, 0);
	auto mouseMove = gameContext.pInput->GetMouseMovement();
	look.x = static_cast<float>(mouseMove.x);
	look.y = static_cast<float>(mouseMove.y);

	m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
	m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();

	m_pCamera->GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

	//MOVEMENT VECTOR
	XMFLOAT3 direction = XMFLOAT3(0, 0, 0);
	direction.z = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD) ? 1.0f : 0.0f;
	if (direction.z == 0) direction.z = -(gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD) ? 1.0f : 0.0f);

	direction.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT) ? 1.0f : 0.0f;
	if (direction.x == 0) direction.x = -(gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT) ? 1.0f : 0.0f);

	//RUN VELOCITY
	if (m_RunVelocity < m_MaxRunVelocity) m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
	else m_RunVelocity = m_MaxRunVelocity;

	auto velocityY = m_Velocity.y;

	XMStoreFloat3(&m_Velocity, (XMLoadFloat3(&direction) * m_RunVelocity));
	m_Velocity.y = velocityY;

	if (!m_pController->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		if (m_JumpVelocity > -m_TerminalVelocity) m_JumpVelocity -= m_JumpAcceleration * gameContext.pGameTime->GetElapsed();
		else m_JumpVelocity = -m_TerminalVelocity;
	}
	else if (gameContext.pInput->IsActionTriggered(CharacterMovement::JUMP))
	{
		m_JumpVelocity = 0;
		m_Velocity.y = 200;
	}
	else
	{
		m_Velocity.y = 0;
	}

	m_Velocity.y += m_JumpVelocity;

	XMFLOAT3 finalVelocity = XMFLOAT3(0.f, 0.f, 0.f);
	XMStoreFloat3(&finalVelocity, XMLoadFloat3(&m_Velocity) * gameContext.pGameTime->GetElapsed());

	m_pController->Move(finalVelocity);
}