#include "stdafx.h"
#include "Scene_MainLevel.h"

#include "../OverlordProject/GAME/Prefabs/PlayerPrefab.h"
#include "Prefabs/FixedCamera.h"
#include "Components/Components.h"
#include "Physx/PhysxManager.h"
#include "../OverlordProject/GAME/Prefabs/BaseEnemy.h"
#include "../OverlordProject/GAME/Prefabs/SmallInvader.h"
#include "../OverlordProject/GAME/Prefabs/LaserPrefab.h"
#include "../OverlordProject/GAME/Prefabs/BlockPrefab.h"
#include "../OverlordProject/GAME/Resources/Materials/MeshMaterial.h"
#include "../OverlordProject/GAME/Prefabs/EnemyLaserPrefab.h"
#include "Graphics/TextRenderer.h"
#include "Content/ContentManager.h"
#include "../OverlordProject/GAME/Prefabs/PickupPrefab.h"
#include <ctime>
#include "Prefabs/SkyBoxPrefab.h"
#include "Components/SpriteComponent.h"
#include "../OverlordProject/GAME/Resources/Materials/MeshMaterial_Skinned.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Explode.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Fall.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Disappear.h"
#include "../OverlordProject/GAME/Prefabs/BaseButtonPrefab.h"
#include "Base/OverlordGame.h"


Scene_MainLevel::Scene_MainLevel(): GameScene(L"Scene_MainLevel")
{
}


Scene_MainLevel::~Scene_MainLevel()
{
}

void Scene_MainLevel::Initialize(const GameContext& gameContext)
{
	srand(time(NULL)); srand(time(NULL)); srand(time(NULL));
	UNREFERENCED_PARAMETER(gameContext);

	//SHADOW
	gameContext.pShadowMapper->SetLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	//CREATE DEFAULT MATERIAL
	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);

	//SET CAMERA
	auto camera = new FixedCamera();
	AddChild(camera);

	camera->GetTransform()->Translate(0.f, 80.f, -57.5f);
	camera->GetTransform()->Rotate(35.f, 0.f, 0.f);

	SetActiveCamera(camera->GetComponent<CameraComponent>());

	//ADD PLAYER
	m_pPlayer = new PlayerPrefab();
	AddChild(m_pPlayer);

	m_pPlayer->GetTransform()->Translate(0, 0, 10);

	//ADD TERRAIN
	auto terrain1 = new GameObject();
	terrain1->AddComponent(new TerrainComponent(L"./GAME/Resources/Terrain.raw", 64, 64, 175, 150, 25));
	AddChild(terrain1);
	terrain1->GetTransform()->Translate(0, -10, 0);

	m_pTerrain.push_back(terrain1);

	auto terrain2 = new GameObject();
	terrain2->AddComponent(new TerrainComponent(L"./GAME/Resources/Terrain.raw", 64, 64, 175, 150, 25));
	AddChild(terrain2);
	terrain2->GetTransform()->Translate(0, -10, 145);

	m_pTerrain.push_back(terrain2);

	auto terrain3 = new GameObject();
	terrain3->AddComponent(new TerrainComponent(L"./GAME/Resources/Terrain.raw", 64, 64, 175, 150, 25));
	AddChild(terrain3);
	terrain3->GetTransform()->Translate(0, -10, 290);

	m_pTerrain.push_back(terrain3);

	auto terrain4 = new GameObject();
	terrain4->AddComponent(new TerrainComponent(L"./GAME/Resources/Terrain.raw", 64, 64, 175, 150, 25));
	AddChild(terrain4);
	terrain4->GetTransform()->Translate(0, -10, 435);

	m_pTerrain.push_back(terrain4);

	//ADD BORDERS
	m_pLaserKill = new GameObject();
	m_pLaserKill->SetTag(L"Backstop");

	m_pLaserKill->AddComponent(new RigidBodyComponent());
	m_pLaserKill->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	shared_ptr<PxGeometry> rectGeom(new PxBoxGeometry(80.f, 2.5f, 2.f));
	m_pLaserKill->AddComponent(new ColliderComponent(rectGeom, *defaultMaterial, PxTransform(PxIdentity)));
	
	AddChild(m_pLaserKill);
	m_pLaserKill->GetTransform()->Translate(0.f, 0.f, 200.f);
	
	m_pFrontStop = new GameObject();
	m_pFrontStop->SetTag(L"FrontStop");

	m_pFrontStop->AddComponent(new RigidBodyComponent());
	m_pFrontStop->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	m_pFrontStop->AddComponent(new ColliderComponent(rectGeom, *defaultMaterial, PxTransform(PxIdentity)));

	AddChild(m_pFrontStop);
	m_pFrontStop->GetTransform()->Translate(0.f, 0.f, -14.f);

	m_pLeftStop = new GameObject();
	m_pLeftStop->SetTag(L"LeftStop");

	m_pLeftStop->AddComponent(new RigidBodyComponent());
	m_pLeftStop->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	shared_ptr<PxGeometry> stopGeom(new PxBoxGeometry(2.5f, 2.5f, 100.f));
	m_pLeftStop->AddComponent(new ColliderComponent(stopGeom, *defaultMaterial, PxTransform(PxIdentity)));

	AddChild(m_pLeftStop);
	m_pLeftStop->GetTransform()->Translate(-65.f, 0.f, 100.f);

	m_pRightStop = new GameObject();
	m_pRightStop->SetTag(L"RightStop");

	m_pRightStop->AddComponent(new RigidBodyComponent());
	m_pRightStop->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	m_pRightStop->AddComponent(new ColliderComponent(stopGeom, *defaultMaterial, PxTransform(PxIdentity)));

	AddChild(m_pRightStop);
	m_pRightStop->GetTransform()->Translate(62.5f, 0.f, 100.f);

	auto middle = new GameObject();
	middle->SetTag(L"MiddleStop");

	middle->AddComponent(new RigidBodyComponent());
	middle->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	middle->AddComponent(new ColliderComponent(rectGeom, *defaultMaterial, PxTransform(PxIdentity)));

	AddChild(middle);
	middle->GetTransform()->Translate(0.f, 0.f, 30.f);

	//ADD BLOCKS
	auto blockLeftDisappear = new Voxelizer_Disappear();
	blockLeftDisappear->SetTime(0.f);
	blockLeftDisappear->SetCubeSize(5.f);
	blockLeftDisappear->SetDiffuseColor(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	blockLeftDisappear->SetOpacityFactor(1.f);
	gameContext.pMaterialManager->AddMaterial(blockLeftDisappear, 40);

	auto blockRightDisappear = new Voxelizer_Disappear();
	blockRightDisappear->SetTime(0.f);
	blockRightDisappear->SetCubeSize(5.f);
	blockRightDisappear->SetDiffuseColor(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	blockRightDisappear->SetOpacityFactor(1.f);
	gameContext.pMaterialManager->AddMaterial(blockRightDisappear, 41);

	auto meshMat1 = new MeshMaterial();
	meshMat1->SetDiffuseColor(XMFLOAT4(0.f, 1.0f, 0.f, 1.0f));
	meshMat1->SetDiffuseTexture(L"./GAME/Resources/Textures/Block_Texture.png");
	meshMat1->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(meshMat1, 10);
	m_pBlockLeft = new BlockPrefab(XMFLOAT3(-30.f, -2.5f, 25.f), 10, 40);
	AddChild(m_pBlockLeft);

	auto meshMat2 = new MeshMaterial();
	meshMat2->SetDiffuseColor(XMFLOAT4(0.f, 1.0f, 0.f, 1.0f));
	meshMat2->SetDiffuseTexture(L"./GAME/Resources/Textures/Block_Texture.png");
	meshMat2->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(meshMat2, 11);
	m_pBlockRight = new BlockPrefab(XMFLOAT3(30.f, -2.5f, 25.f), 11, 41);
	AddChild(m_pBlockRight);

	//ADD HUD
	m_pHUD_Health_Back = new GameObject();
	m_pHUD_Health_Back->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Health_Back.png"));
	AddChild(m_pHUD_Health_Back);

	m_pHUD_Health_Back->GetTransform()->Translate(20, 20, 0);

	m_pHUD_Health_Front = new GameObject();
	m_pHUD_Health_Front->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Health_Front.png"));
	AddChild(m_pHUD_Health_Front);

	m_pHUD_Health_Front->GetTransform()->Translate(22, 22, 0);

	m_pHUD_Heart = new GameObject();
	m_pHUD_Heart->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Heart.png"));
	AddChild(m_pHUD_Heart);

	m_pHUD_Heart->GetTransform()->Translate(90, 10, 0);

	m_pHUD_Shield = new GameObject();
	m_pHUD_Shield->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Shield.png"));
	AddChild(m_pHUD_Shield);

	m_pHUD_Shield->GetTransform()->Translate(20, 80, 0);

	m_pHUD_Shield_Front = new GameObject();
	m_pHUD_Shield_Front->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Shield_Front.png"));
	AddChild(m_pHUD_Shield_Front);

	m_pHUD_Shield_Front->GetTransform()->Translate(72, 82, 0);

	m_pHUD_Shield_Back = new GameObject();
	m_pHUD_Shield_Back->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Shield_Back.png"));
	AddChild(m_pHUD_Shield_Back);

	m_pHUD_Shield_Back->GetTransform()->Translate(70, 80, 0);

	m_pHUD_Score_Back = new GameObject();
	m_pHUD_Score_Back->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Score_Back.png"));
	AddChild(m_pHUD_Score_Back);

	m_pHUD_Score_Back->GetTransform()->Translate(1030, 10, 0);

	m_pHUD_Score_Front = new GameObject();
	m_pHUD_Score_Front->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/HUD/HUD_Score_Front.png"));
	AddChild(m_pHUD_Score_Front);

	m_pHUD_Score_Front->GetTransform()->Translate(1020, 20, 0);

	//ADD FONT
	m_pHUD_Font32 = ContentManager::Load<SpriteFont>(L"./GAME/Resources/Font/Demonized_32.fnt");
	m_pHUD_Font64 = ContentManager::Load<SpriteFont>(L"./GAME/Resources/Font/Demonized_64.fnt");

	//ADD SKYBOX
	auto skybox = new SkyBoxPrefab();
	AddChild(skybox);

	//CREATE MATERIALS
	auto laserLeftExplode = new Voxelizer_Explode();
	laserLeftExplode->SetTime(0.f);
	laserLeftExplode->SetCubeSize(0.5f);
	laserLeftExplode->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 0.f, 1.f));
	laserLeftExplode->SetExplosionForce(50.f);
	laserLeftExplode->SetGravity(9.18f);
	laserLeftExplode->SetMaxTime(1.f);
	laserLeftExplode->SetSlowMotion(1.0f);
	gameContext.pMaterialManager->AddMaterial(laserLeftExplode, 50);

	auto laserRightExplode = new Voxelizer_Explode();
	laserRightExplode->SetTime(0.f);
	laserRightExplode->SetCubeSize(0.5f);
	laserRightExplode->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 0.f, 1.f));
	laserRightExplode->SetExplosionForce(50.f);
	laserRightExplode->SetGravity(9.18f);
	laserRightExplode->SetMaxTime(1.f);
	laserRightExplode->SetSlowMotion(1.0f);
	gameContext.pMaterialManager->AddMaterial(laserRightExplode, 51);

	for (size_t i = 0; i < 7; i++)
	{
		auto laserEnemyExplode = new Voxelizer_Explode();
		laserEnemyExplode->SetTime(0.f);
		laserEnemyExplode->SetCubeSize(0.5f);
		laserEnemyExplode->SetDiffuseColor(XMFLOAT4(5.f, 0.f, 0.f, 1.f));
		laserEnemyExplode->SetExplosionForce(50.f);
		laserEnemyExplode->SetGravity(9.18f);
		laserEnemyExplode->SetMaxTime(1.f);
		laserEnemyExplode->SetSlowMotion(1.0f);
		gameContext.pMaterialManager->AddMaterial(laserEnemyExplode, 52 + i);
	}

	for (size_t i = 0; i < 7 * 4; i++)
	{
		auto enemyFall = new Voxelizer_Fall();
		enemyFall->SetTime(0.f);
		enemyFall->SetCubeSize(5.f);
		enemyFall->SetDiffuseColor(XMFLOAT4(5.f, 0.f, 0.f, 1.f));
		enemyFall->SetGravity(100.f); 
		enemyFall->SetMaxTime(5.f);
		enemyFall->SetSlowMotion(1.f);
		gameContext.pMaterialManager->AddMaterial(enemyFall, 59 + i);

	}

	auto pickupMat1 = new MeshMaterial();
	pickupMat1->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Health.png");
	pickupMat1->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 5.f, 1.f));
	pickupMat1->SetInverted(false);
	pickupMat1->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pickupMat1, 12);

	auto pickupMat2 = new MeshMaterial();
	pickupMat2->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Shield.png");
	pickupMat2->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 0.f, 1.f));
	pickupMat2->SetInverted(false);
	pickupMat2->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pickupMat2, 13);

	auto pickupMat3 = new MeshMaterial();
	pickupMat3->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Rebuild.png");
	pickupMat3->SetDiffuseColor(XMFLOAT4(5.f, 5.f, 0.f, 1.f));
	pickupMat3->SetInverted(false);
	pickupMat3->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pickupMat3, 14);

	auto pickupMat4 = new MeshMaterial();
	pickupMat4->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Full.png");
	pickupMat4->SetDiffuseColor(XMFLOAT4(5.f, 5.f, 5.f, 1.f));
	pickupMat4->SetInverted(false);
	pickupMat4->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(pickupMat4, 15);

	auto laserMat = new MeshMaterial();
	laserMat->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Full.png");
	laserMat->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 0.f, 1.f));
	laserMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(laserMat, 16);

	auto enemyLaserMat = new MeshMaterial();
	enemyLaserMat->SetDiffuseTexture(L"./GAME/Resources/Textures/PickUp_Full.png");
	enemyLaserMat->SetDiffuseColor(XMFLOAT4(5.f, 0.f, 0.f, 1.f));
	enemyLaserMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(enemyLaserMat, 17);

	auto enemyMat = new MeshMaterial_Skinned();
	enemyMat->SetDiffuseTexture(L"./GAME/Resources/Textures/Enemy_Texture.png");
	enemyMat->SetDiffuseColor(XMFLOAT4(5.f, 0.f, 0.f, 1.f));
	enemyMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(enemyMat, 18);

	//Button Input

	gameContext.pInput->AddInputAction(InputAction(10, Pressed, -1, VK_LBUTTON));
}

void Scene_MainLevel::Update(const GameContext& gameContext)
{
	if (m_pPlayer->IsDead())
	{
		m_End = true;
	}

	UNREFERENCED_PARAMETER(gameContext);
	//Spawn Enemies
	SpawnEnemies(gameContext);

	ManageTerrain(gameContext);
	ManageLasers(gameContext);
	ManageEnemies(gameContext);
	ManagePickUps(gameContext);
	ManageBlocks(gameContext);
	ManageHUD(gameContext);


	if (gameContext.pInput->IsActionTriggered(PlayerPrefab::SHOOTPULSE))
	{
		auto laser = m_pPlayer->ShootLeft(gameContext, XMFLOAT3(0, 0, 1), 200);
		if (laser != nullptr)
		{
			AddChild(laser);
			m_pLasers.push_back(laser);
		}

		auto laser2 = m_pPlayer->ShootRight(gameContext, XMFLOAT3(0, 0, 1), 200);
		if (laser2 != nullptr)
		{
			AddChild(laser2);
			m_pLasers.push_back(laser2);

			m_pPlayer->SetTimer(0.0f);
		}
	}

	if (m_End && m_Buttons == 0)
	{
		m_pExit = new BaseButtonPrefab({ 500,425 }, L"./GAME/Resources/Textures/Menu's/Button_Exit.png");

		m_pExit->SetButtonCallback([this]()
		{
			OverlordGame::WindowsProcedureStatic(OverlordGame::GetGameSettings().Window.WindowHandle, WM_DESTROY, NULL, NULL);
		});

		AddChild(m_pExit);
		m_Buttons += 1;
	}

	if (m_End)
	{
		auto mousePos = gameContext.pInput->GetMousePosition();
		if (gameContext.pInput->IsActionTriggered(10))
		{
			if (mousePos.x > m_pExit->GetPosition().x &&
				mousePos.x < m_pExit->GetPosition().x + m_pExit->GetWidth() &&
				mousePos.y > m_pExit->GetPosition().y &&
				mousePos.y < m_pExit->GetPosition().y + m_pExit->GetHeight())
			{
				m_pExit->OnClick();
			}
		}

		if (mousePos.x > m_pExit->GetPosition().x &&
			mousePos.x < m_pExit->GetPosition().x + m_pExit->GetWidth() &&
			mousePos.y > m_pExit->GetPosition().y &&
			mousePos.y < m_pExit->GetPosition().y + m_pExit->GetHeight())
		{
			m_pExit->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Inverted_Exit.png");
		}
		else
		{
			m_pExit->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Exit.png");
		}
	}
}

void Scene_MainLevel::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	TextRenderer::GetInstance()->DrawText(m_pHUD_Font64, std::to_wstring(int(m_pPlayer->GetHealth())) + L"%", { 5.f, -5.f }, XMFLOAT4(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pHUD_Font32, std::to_wstring(int(m_pPlayer->GetShield())) + L"%", { 400.f, 90.f }, XMFLOAT4(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pHUD_Font64, std::to_wstring(m_Score), { 1035, 20 });
	
	if (m_End)
	{
		TextRenderer::GetInstance()->DrawText(m_pHUD_Font64, L"Score: " + std::to_wstring(m_Score), { 500,300 }, XMFLOAT4(Colors::White));
		TextRenderer::GetInstance()->DrawText(m_pHUD_Font64, L"YOU DIED", { 500,350 }, XMFLOAT4(Colors::White));
	}
}

void Scene_MainLevel::ManageLasers(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	LaserPrefab* toDelete = nullptr;

	for (LaserPrefab* laser : m_pLasers)
	{
		if (!laser->GetActive())
		{
			toDelete = laser;
		}
	}

	RemoveFromVec <LaserPrefab> (toDelete, m_pLasers);

	EnemyLaserPrefab* toDelete2 = nullptr;

	for (EnemyLaserPrefab* laser : m_pEnemyLasers)
	{
		if (!laser->GetActive())
		{
			toDelete2 = laser;
		}
	}

	RemoveFromVec <EnemyLaserPrefab>(toDelete2, m_pEnemyLasers);
}

void Scene_MainLevel::ManageEnemies(const GameContext& gameContext)
{
	SmallInvader* toDelete = nullptr;

	for (SmallInvader* enemy : m_pEnemies)
	{
		if (!enemy->GetActive())
		{
			m_Score += 100;
			toDelete = enemy;
			SpawnPickUps(gameContext);
		}
		else
		{
			enemy->Movement(gameContext);
		 	auto laser = enemy->Attack(gameContext);
			if (laser)
			{
				m_pEnemyLasers.push_back(laser);
				AddChild(laser);
			}
		}
	}

	RemoveFromVec(toDelete, m_pEnemies);

	if (m_pEnemies.size() < 15)
	{
		if (m_EnemyLevel < 4)
		{
			for (auto enemy : m_pEnemies)
			{
				enemy->SetMoveSpeed(25.f);
				enemy->SetShootInterval(2.5f);
				enemy->SetShootSpeed(275.f);
			}
		}
		else
		{
			for (auto enemy : m_pEnemies)
			{
				enemy->SetMoveSpeed(30.f);
				enemy->SetShootInterval(1.25f);
				enemy->SetShootSpeed(325.f);
			}
		}
	}

	if (m_pEnemies.size() > 0)
	{
		auto lastEnemy = reinterpret_cast<SmallInvader*>(m_pEnemies.back());
		if (lastEnemy->GetControllerPosition().z <= 60.F)
		{
			for (SmallInvader* enemy : m_pEnemies)
			{
				reinterpret_cast<SmallInvader*>(enemy)->SetMoveZ(false);
			}
		}
		else
		{
			for (SmallInvader* enemy : m_pEnemies)
			{
				reinterpret_cast<SmallInvader*>(enemy)->SetMoveZ(true);
			}
		}
	}
}

void Scene_MainLevel::ManageBlocks(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_pBlockLeft != nullptr)
	{
		if (!m_pBlockLeft->GetActive())
		{
			RemoveChild(m_pBlockLeft);
			m_pBlockLeft = nullptr;
		}
	}

	if (m_pBlockRight != nullptr)
	{
		if (!m_pBlockRight->GetActive())
		{
			RemoveChild(m_pBlockRight);
			m_pBlockRight = nullptr;
		}
	}
}

void Scene_MainLevel::ManageTerrain(const GameContext& gameContext)
{
	for (auto terrain : m_pTerrain)
	{
		auto position = terrain->GetTransform()->GetPosition();
		if (position.z <= -80)
		{
			terrain->GetTransform()->Translate(0, -10, 435);
			continue;
		}

		position.z -= 50 * gameContext.pGameTime->GetElapsed();

		terrain->GetTransform()->Translate(position);
	}
}

void Scene_MainLevel::ManageHUD(const GameContext& gameContext) const
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pHUD_Health_Front->GetTransform()->Scale(m_pPlayer->GetHealth() / 100, 1, 1);
	m_pHUD_Shield_Front->GetTransform()->Scale(m_pPlayer->GetShield() / 100, 1, 1);
}

void Scene_MainLevel::ManagePickUps(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	PickupPrefab* toDelete = nullptr;

	for (PickupPrefab* pickup : m_pPickups)
	{
		if (!pickup->GetActive())
		{
			if (pickup->GetRebuild())
			{
				if (m_pBlockLeft == nullptr)
				{
					m_pBlockLeft = new BlockPrefab(XMFLOAT3(-30.f, -2.5f, 25.f), 10, 40);
					AddChild(m_pBlockLeft);
				}
				else if (m_pBlockRight == nullptr)
				{
					m_pBlockRight = new BlockPrefab(XMFLOAT3(30.f, -2.5f, 25.f), 11, 41);
					AddChild(m_pBlockRight);
				}
				pickup->SetRebuild(false);
			}

			toDelete = pickup;
		}
	}

	RemoveFromVec <PickupPrefab> (toDelete, m_pPickups);
}

void Scene_MainLevel::SpawnEnemies(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_pEnemies.size() == 0)
	{
		m_EnemyLevel += 1;
		float x = -40.f;
		float z = 180.f;
		auto matID = 0;
		auto laserMatID = 0;

		if (m_EnemyLevel < 4)
		{
			for (int i = 0; i < 7 * 4; i++)
			{
				auto enemy = new SmallInvader(XMFLOAT3(x, 0, z), 15.f, 10.f, 15.f,59 + laserMatID,52 + laserMatID);
				x += 13.f;
				matID += 1;
				laserMatID += 1;
				enemy->SetShootInterval(3.75f);
				enemy->SetShootSpeed(250.f);

				m_pEnemies.push_back(enemy);

				if ((i + 1) % 7 == 0)
				{
					z -= 20.f;
					x = -40;
					matID = 0;
					laserMatID = 0;
				}

				AddChild(enemy);
			}
		}
		else
		{
			for (int i = 0; i < 7 * 4; i++)
			{
				auto enemy = new SmallInvader(XMFLOAT3(x, 0, z), 15.f, 10.f, 20.f,59 + laserMatID,52 + laserMatID);
				x += 13.f;
				matID += 1;
				laserMatID += 1;
				enemy->SetShootInterval(2.5f);
				enemy->SetShootSpeed(300.f);

				m_pEnemies.push_back(enemy);

				if ((i + 1) % 7 == 0)
				{
					z -= 20.f;
					x = -40;
					matID = 0;
					laserMatID = 0;
				}

				AddChild(enemy);
			}
		}
	}
}

void Scene_MainLevel::SpawnPickUps(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto randPickup = rand() % 4;
	
	if (randPickup == PickupPrefab::PickupType::HEALTH)
	{
		if (m_pPlayer->GetHealth() != 100)
		{
			auto randSpawn = rand() % 5;
			if (randSpawn == 0)
			{
				auto pickup = new PickupPrefab(XMFLOAT3(0, 5.f, 0), PickupPrefab::PickupType::HEALTH);
				AddChild(pickup);
	
				m_pPickups.push_back(pickup);
			}
		}
	}
	else if (randPickup == PickupPrefab::PickupType::SHIELD)
	{
		if (m_pPlayer->GetShield() != 100)
		{
			auto randSpawn = rand() % 3;
			if (randSpawn == 0)
			{
				auto pickup = new PickupPrefab(XMFLOAT3(0, 5.f, 0), PickupPrefab::PickupType::SHIELD);
				AddChild(pickup);
	
				m_pPickups.push_back(pickup);
			}
		}
	}
	else if (randPickup == PickupPrefab::PickupType::REBUILD)
	{
		if (m_pBlockLeft == nullptr || m_pBlockRight == nullptr)
		{
			auto randSpawn = rand() % 2;
			if (randSpawn == 0)
			{
				auto pickup = new PickupPrefab(XMFLOAT3(0, 5.f, 0), PickupPrefab::PickupType::REBUILD);
				AddChild(pickup);
	
				m_pPickups.push_back(pickup);
			}
		}
	}
	else if (randPickup == PickupPrefab::PickupType::RAPIDFIRE)
	{
		auto randSpawn = rand() % 10;
		if (randSpawn == 0)
		{
			auto pickup = new PickupPrefab(XMFLOAT3(0, 5.f, 0), PickupPrefab::PickupType::RAPIDFIRE);
			AddChild(pickup);
	
			m_pPickups.push_back(pickup);
		}
	}
	
}
