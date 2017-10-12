#include "stdafx.h"
#include "PongScene.h"
#include "Physx/PhysxProxy.h"
#include "Physx/PhysxManager.h"
#include "Components/Components.h"
#include "Scenegraph/GameObject.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/FixedCamera.h"
#include "Prefabs/SpherePrefab.h"
#include <ctime>


PongScene::PongScene() : GameScene(L"PongScene")
{
}

PongScene::~PongScene()
{
}

void PongScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//PHYSX + GROUND
	//**************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.f, 0.f, 1.f);
	auto paddleMaterial = physX->createMaterial(0.f, 0.f, 1.f);

	auto pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));

	AddChild(pGround);

	//CAMERA
	//******
	auto camera = new FixedCamera();
	AddChild(camera);

	camera->GetTransform()->Translate(0.f, 60.f, 0.f);
	camera->GetTransform()->Rotate(90.f, 0.f, 0.f);

	SetActiveCamera(camera->GetComponent<CameraComponent>());

	//BORDERS
	//*******

	auto borderTop = new GameObject();
	auto pRigidB = new RigidBodyComponent();
	pRigidB->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ | TransZ), true);
	borderTop->AddComponent(pRigidB);

	auto borderBot = new GameObject();
	pRigidB = new RigidBodyComponent();
	pRigidB->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ | TransZ), true);
	borderBot->AddComponent(pRigidB);

	std::shared_ptr<PxGeometry> rectGeom(new PxBoxGeometry(0.5f, 40.f, 0.5f));
	borderTop->AddComponent(new ColliderComponent(rectGeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	borderBot->AddComponent(new ColliderComponent(rectGeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(borderTop);
	AddChild(borderBot);

	borderTop->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	borderBot->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	borderTop->GetTransform()->Translate(0.f, 5.f, 22.f);
	borderBot->GetTransform()->Translate(0.f, 5.f, -22.f);

	borderTop->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	borderBot->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	//TRIGGERS (GOALS)
	//****************

	m_TriggerLeft = new GameObject();
	pRigidB = new RigidBodyComponent();
	pRigidB->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ), true);
	m_TriggerLeft->AddComponent(pRigidB);

	std::shared_ptr<PxGeometry> triggerGeom(new PxBoxGeometry(0.5f, 0.5f, 22.f));
	auto colliderCmp = new ColliderComponent(triggerGeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1))));
	colliderCmp->EnableTrigger(true);

	m_TriggerLeft->AddComponent(colliderCmp);

	m_TriggerRight = new GameObject();
	pRigidB = new RigidBodyComponent();
	pRigidB->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ), true);
	m_TriggerRight->AddComponent(pRigidB);

	colliderCmp = new ColliderComponent(triggerGeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1))));
	colliderCmp->EnableTrigger(true);

	m_TriggerRight->AddComponent(colliderCmp);

	m_TriggerLeft->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);

		if (action == GameObject::TriggerAction::ENTER && other->GetTag() == L"Ball")
		{
			m_Speed = 30.f;
			m_Ball->GetTransform()->Translate(0.f, 5.f, 0.f);
			m_Elapsed = 0.f;
			m_Reset = true;
			m_ScoreGreen += 1;
			Logger::LogInfo(L"Green Scored! Total: " + std::to_wstring(m_ScoreGreen), false);
		}
	});

	m_TriggerRight->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);

		if (action == GameObject::TriggerAction::ENTER && other->GetTag() == L"Ball")
		{
			m_Speed = 30.f;
			m_Ball->GetTransform()->Translate(0.f, 5.f, 0.f);
			m_Elapsed = 0.f;
			m_Reset = true;
			m_ScoreRed += 1;
			Logger::LogInfo(L"Red Scored! Total: " + std::to_wstring(m_ScoreRed), false);
		}
	});

	AddChild(m_TriggerLeft);
	AddChild(m_TriggerRight);

	m_TriggerLeft->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	m_TriggerRight->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_TriggerLeft->GetTransform()->Translate(-40.f, 5.f, 0.f);
	m_TriggerRight->GetTransform()->Translate(40.f, 5.f, 0.f);

	//BACKGROUND
	//**********
	auto pBackground = new CubePrefab(90, 1, 60, XMFLOAT4(Colors::DarkSlateGray));
	AddChild(pBackground);

	//PADDLES
	//*******
	m_Paddle1 = new CubePrefab(1, 2, 12, XMFLOAT4(Colors::Red));

	auto pRigid = new RigidBodyComponent();
	pRigid->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ), true);
	pRigid->SetKinematic(true);
	m_Paddle1->AddComponent(pRigid);

	std::shared_ptr<PxGeometry> cubeGeom(new PxBoxGeometry(1.f, 0.5f, 6.f));
	m_Paddle1->AddComponent(new ColliderComponent(cubeGeom, *paddleMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));

	AddChild(m_Paddle1);

	m_Paddle2 = new CubePrefab(1, 2, 12, XMFLOAT4(Colors::Green));

	pRigid = new RigidBodyComponent();
	pRigid->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ), true);
	pRigid->SetKinematic(true);
	m_Paddle2->AddComponent(pRigid);

	m_Paddle2->AddComponent(new ColliderComponent(cubeGeom, *paddleMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));

	AddChild(m_Paddle2);

	m_Paddle1->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	m_Paddle2->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_Paddle1->GetTransform()->Translate(-37.5f, 5.f, 0.f);
	m_Paddle2->GetTransform()->Translate(37.5f, 5.f, 0.f);

	//BALL
	//****
	m_Ball = new SpherePrefab(1, 20, XMFLOAT4(Colors::Purple));

	pRigid = new RigidBodyComponent();
	pRigid->SetConstraint(static_cast<RigidBodyConstraintFlag>(RotX | RotY | RotZ), true);
	m_Ball->AddComponent(pRigid);

	std::shared_ptr<PxGeometry> sphereGeom(new PxSphereGeometry(0.5f));
	m_Ball->AddComponent(new ColliderComponent(sphereGeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));

	AddChild(m_Ball);

	m_Ball->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	m_Ball->SetTag(L"Ball");
	m_Ball->GetTransform()->Translate(0.f, 5.f, 0.f);

	std::srand(static_cast<unsigned> (time(0)));
	PxVec3 dir = PxVec3((static_cast<double>(rand()) / (RAND_MAX)) + 0.3f, 0.f, (static_cast<double> (rand()) / (RAND_MAX)));
	m_Ball->GetComponent<RigidBodyComponent>()->AddForce(dir * m_Speed, PxForceMode::eIMPULSE, true);

	//INPUT
	//*****
	gameContext.pInput->AddInputAction(InputAction(1, Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(2, Down, 'I'));
	gameContext.pInput->AddInputAction(InputAction(3, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(4, Down, 'K'));
}

void PongScene::Update(const GameContext& gameContext)
{
	if (m_Reset == true)
	{
		//PxVec3 dir = PxVec3((static_cast<double>(rand()) / (RAND_MAX)) + 0.3f, 0.f, (static_cast<double> (rand()) / (RAND_MAX)) - 0.5f);
		//m_Ball->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->clearForce(PxForceMode::eVELOCITY_CHANGE);
		//m_Ball->GetComponent<RigidBodyComponent>()->AddForce(dir * m_Speed, PxForceMode::eIMPULSE, true);
		m_Reset = false;
	}

	if (gameContext.pInput->IsActionTriggered(1) && m_PaddlePos1 < 15)
	{
		m_PaddlePos1 += 20.0f * gameContext.pGameTime->GetElapsed();
	}
	else if (gameContext.pInput->IsActionTriggered(3) && m_PaddlePos1 > -15)
	{
		m_PaddlePos1 -= 20.0f * gameContext.pGameTime->GetElapsed();
	}

	if (gameContext.pInput->IsActionTriggered(2) && m_PaddlePos2 < 15)
	{
		m_PaddlePos2 += 20.0f * gameContext.pGameTime->GetElapsed();
	}
	else if (gameContext.pInput->IsActionTriggered(4) && m_PaddlePos2 > -15)
	{
		m_PaddlePos2 -= 20.0f * gameContext.pGameTime->GetElapsed();
	}

	m_Paddle1->GetTransform()->Translate(-37.5f, 5.f, m_PaddlePos1);
	m_Paddle2->GetTransform()->Translate(37.5f, 5.f, m_PaddlePos2);

	m_Elapsed += gameContext.pGameTime->GetElapsed();

	if (m_Elapsed >= 10.f && m_Speed < 100.f)
	{
		auto dir = m_Ball->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->getLinearVelocity().getNormalized();
		m_Speed += 10;
		m_Ball->GetComponent<RigidBodyComponent>()->GetPxRigidBody()->setLinearVelocity(dir * m_Speed, true);
		m_Elapsed = 0.f;
	}
}

void PongScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void PongScene::SceneActivated()
{
}

void PongScene::SceneDeactivated()
{
}
