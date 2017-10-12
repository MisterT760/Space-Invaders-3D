#include "stdafx.h"
#include "HeightmapTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Physx/PhysxProxy.h"
#include "Prefabs/SpherePrefab.h"
#include "Physx/PhysxManager.h"


HeightmapTestScene::HeightmapTestScene() : GameScene(L"HeightmapTestScene")
{
}


HeightmapTestScene::~HeightmapTestScene()
{
}

void HeightmapTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.1f, 0.1f, 0.1f);

	auto terrain = new GameObject();
	terrain->AddComponent(new TerrainComponent(L"./Resources/Terrain/Hawai_HeightMap_64x64x16.raw", 64, 64, 64, 64, 16));
	AddChild(terrain);

	auto pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 30, 0);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0); //SET GROUP
	pSphere->AddComponent(pRigidBody);

	std::shared_ptr<PxGeometry> spheregeom(new PxSphereGeometry(1));
	pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(pSphere);
}

void HeightmapTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	Logger::LogInfo(std::to_wstring(gameContext.pGameTime->GetFPS()), false);
}

void HeightmapTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
