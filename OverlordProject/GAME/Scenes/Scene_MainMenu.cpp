#include "stdafx.h"
#include "Scene_MainMenu.h"
#include "Prefabs/FixedCamera.h"
#include "Components/Components.h"
#include "Base/OverlordGame.h"
#include "../OverlordProject/GAME/Prefabs/BaseButtonPrefab.h"
#include "Scenegraph/SceneManager.h"
#include "../OverlordProject/GAME/Resources/Materials/MeshMaterial.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Diagnostics/DebugRenderer.h"
#include "../OverlordProject/Materials/Post/PostCA.h"
#include "../OverlordProject/Materials/Post/PostBlur.h"
#include "../OverlordProject/Materials/Post/PostGrayscale.h"
#include "../OverlordProject/GAME/Prefabs/ClothPlane.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "../OverlordProject/Materials/Shadow/DiffuseMaterial_Shadow.h"


Scene_MainMenu::Scene_MainMenu(): GameScene(L"Scene_MainMenu"), m_pButton_Start(nullptr), m_pButton_Help(nullptr), m_pButton_Exit(nullptr), m_pShip(nullptr), m_pGameName(nullptr)
{
}


Scene_MainMenu::~Scene_MainMenu()
{
}

void Scene_MainMenu::Initialize(const GameContext& gameContext)
{
	DebugRenderer::ToggleDebugRenderer();
	
	gameContext.pShadowMapper->SetLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });
	auto defaultMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.f, 0.f, 0.1f);

	//CAMERA
	auto camera = new FixedCamera();
	AddChild(camera);

	camera->GetTransform()->Translate(0.f, 25.f, -125.f);

	SetActiveCamera(camera->GetComponent<CameraComponent>());

	//BUTTONS
	m_pButton_Start = new BaseButtonPrefab({ 50,300 }, L"./GAME/Resources/Textures/Menu's/Button_Start.png");

	m_pButton_Start->SetButtonCallback([this]()
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"Scene_MainLevel");
	});

	AddChild(m_pButton_Start);

	m_pButton_Help = new BaseButtonPrefab({ 50,400 }, L"./GAME/Resources/Textures/Menu's/Button_Help.png");

	m_pButton_Help->SetButtonCallback([this]()
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"Scene_HelpMenu");
	});

	AddChild(m_pButton_Help);

	m_pButton_Exit = new BaseButtonPrefab({ 50,500 }, L"./GAME/Resources/Textures/Menu's/Button_Exit.png");
	m_pButton_Exit->SetButtonCallback([this]()
	{
		OverlordGame::WindowsProcedureStatic(OverlordGame::GetGameSettings().Window.WindowHandle, WM_DESTROY, NULL, NULL );
	});

	AddChild(m_pButton_Exit);

	//SHIP
	m_pShip = new GameObject();
	auto shipMat = new MeshMaterial();
	shipMat->SetDiffuseColor(XMFLOAT4(0.f, 5.f, 5.f, 1.f));
	shipMat->SetDiffuseTexture(L"./GAME/Resources/Textures/Player_Texture.png");
	shipMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(shipMat, 1);

	auto modelComponent = new ModelComponent(L"./GAME/Resources/Meshes/PlayerShip.ovm");
	modelComponent->SetMaterial(1);
	m_pShip->AddComponent(modelComponent);

	m_pShip->AddComponent(new RigidBodyComponent());
	m_pShip->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	auto convexGeom = ContentManager::Load<PxConvexMesh>(L"./GAME/Resources/Meshes/PlayerShip.ovpc");
	shared_ptr<PxGeometry> geometry(new PxConvexMeshGeometry(convexGeom));
	auto colliderComp = new ColliderComponent(geometry, *defaultMaterial);
	m_pShip->AddComponent(colliderComp);

	AddChild(m_pShip);

	m_pShip->GetTransform()->Scale(0.4f, 0.4f, 0.4f);
	m_pShip->GetTransform()->Rotate(10.f, 120.f, -25.f);
	m_pShip->GetTransform()->Translate(60.f, 0.f, 45.f);

	//NAME
	m_pGameName = new GameObject();
	m_pGameName->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/Menu's/Scene_Game_Name.png"));
	m_pGameName->GetTransform()->Translate(250, 50, 0);
	AddChild(m_pGameName);

	//GROUND
	auto diffMat = new DiffuseMaterial_Shadow();
	diffMat->SetDiffuseTexture(L"./GAME/Resources/Textures/Terrain_Texture_Invert.png");
	diffMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat, 21);

	auto pGroundObj = new GameObject();
	auto pGroundModel = new ModelComponent(L"./GAME/Resources/Meshes/MM_Plane.ovm");
	pGroundModel->SetMaterial(21);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);

	AddChild(pGroundObj);

	pGroundObj->GetTransform()->Scale(10, 10, 10);
	pGroundObj->GetTransform()->Rotate(0.f, 90.f, -25.f);
	pGroundObj->GetTransform()->Translate(0.f, -20.f, 0.f);

	//SKYBOX
	auto skybox = new SkyBoxPrefab();
	AddChild(skybox);

	gameContext.pInput->AddInputAction(InputAction(0, Pressed, -1, VK_LBUTTON));
	gameContext.pInput->AddInputAction(InputAction(1, Pressed, '1'));

	m_pChromaticAbboration = new PostCA();
	gameContext.pMaterialManager->AddMaterial_PP(m_pChromaticAbboration, 0);
	AddPostProcessingMaterial(0);

	auto blur = new PostBlur();
	gameContext.pMaterialManager->AddMaterial_PP(blur, 1);
	
	auto gray = new PostGrayscale();
	gameContext.pMaterialManager->AddMaterial_PP(gray, 2);

	std::vector<UINT> points;
	for (int i = 0; i < 64; ++i)
	{
		points.push_back(i);
	}

	auto cloth = new ClothPlane(64, 64, 75, 100, 1, XMFLOAT3(0, 115, 0), points);
	AddChild(cloth);
}

void Scene_MainMenu::Update(const GameContext& gameContext)
{
	m_pChromaticAbboration->SetDistortionTime(m_CATime);

	if (gameContext.pInput->IsActionTriggered(1))
	{
		if (m_BlurGray == false)
		{
			AddPostProcessingMaterial(1);
			AddPostProcessingMaterial(2);
			m_BlurGray = true;
		}
		else
		{
			RemovePostProcessingMaterial(1);
			RemovePostProcessingMaterial(2);
			m_BlurGray = false;
		}
	}

	//Start Button
	auto mousePos = gameContext.pInput->GetMousePosition();
	if (gameContext.pInput->IsActionTriggered(0))
	{
		if (mousePos.x > m_pButton_Start->GetPosition().x &&
			mousePos.x < m_pButton_Start->GetPosition().x + m_pButton_Start->GetWidth() &&
			mousePos.y > m_pButton_Start->GetPosition().y &&
			mousePos.y < m_pButton_Start->GetPosition().y + m_pButton_Start->GetHeight())
		{
			m_pButton_Start->OnClick();
		}
	}

	if (mousePos.x > m_pButton_Start->GetPosition().x &&
		mousePos.x < m_pButton_Start->GetPosition().x + m_pButton_Start->GetWidth() &&
		mousePos.y > m_pButton_Start->GetPosition().y &&
		mousePos.y < m_pButton_Start->GetPosition().y + m_pButton_Start->GetHeight())
	{
		m_pButton_Start->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Inverted_Start.png");
	}
	else
	{
		m_pButton_Start->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Start.png");
	}

	//Help Button
	if (gameContext.pInput->IsActionTriggered(0))
	{
		if (mousePos.x > m_pButton_Help->GetPosition().x &&
			mousePos.x < m_pButton_Help->GetPosition().x + m_pButton_Help->GetWidth() &&
			mousePos.y > m_pButton_Help->GetPosition().y &&
			mousePos.y < m_pButton_Help->GetPosition().y + m_pButton_Help->GetHeight())
		{
			m_pButton_Help->OnClick();
		}
	}


	if (mousePos.x > m_pButton_Help->GetPosition().x &&
		mousePos.x < m_pButton_Help->GetPosition().x + m_pButton_Help->GetWidth() &&
		mousePos.y > m_pButton_Help->GetPosition().y &&
		mousePos.y < m_pButton_Help->GetPosition().y + m_pButton_Help->GetHeight())
	{
		m_pButton_Help->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Inverted_Help.png");
	}
	else
	{
		m_pButton_Help->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Help.png");
	}

	//Exit Button
	if (gameContext.pInput->IsActionTriggered(0))
	{
		if (mousePos.x > m_pButton_Exit->GetPosition().x &&
			mousePos.x < m_pButton_Exit->GetPosition().x + m_pButton_Exit->GetWidth() &&
			mousePos.y > m_pButton_Exit->GetPosition().y &&
			mousePos.y < m_pButton_Exit->GetPosition().y + m_pButton_Exit->GetHeight())
		{
			m_pButton_Exit->OnClick();
		}
	}

	if (mousePos.x > m_pButton_Exit->GetPosition().x &&
		mousePos.x < m_pButton_Exit->GetPosition().x + m_pButton_Exit->GetWidth() &&
		mousePos.y > m_pButton_Exit->GetPosition().y &&
		mousePos.y < m_pButton_Exit->GetPosition().y + m_pButton_Exit->GetHeight())
	{
		m_pButton_Exit->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Inverted_Exit.png");
	}
	else
	{
		m_pButton_Exit->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Exit.png");
	}

	if (m_Timer > 7.5f)
	{
		m_CATimer = m_Timer;
		m_Time += 1;
		m_Timer = 0.f;
		m_CATime = float(m_Time);
	}

	if (m_CATimer > 7.5f)
	{
		m_pChromaticAbboration->SetDistortionOffsets(XMFLOAT3(randF(0.25f, 0.75), randF(0.25f, 0.75), randF(0.25f, 0.75)));
	}

	if (m_CATimer > 8.f)
	{

		m_pChromaticAbboration->SetDistortionOffsets(XMFLOAT3(0, 0, 0));
		m_Timer += gameContext.pGameTime->GetElapsed();
	}

	m_Timer += gameContext.pGameTime->GetElapsed();
	m_CATimer += gameContext.pGameTime->GetElapsed();
	m_CATime += (gameContext.pGameTime->GetElapsed() / 100);
}

void Scene_MainMenu::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
