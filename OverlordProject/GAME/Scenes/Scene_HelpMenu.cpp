#include "stdafx.h"
#include "Scene_HelpMenu.h"
#include "../OverlordProject/GAME/Prefabs/BaseButtonPrefab.h"
#include "Scenegraph/SceneManager.h"
#include "Prefabs/FixedCamera.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Components/ModelComponent.h"
#include "../OverlordProject/Materials/Shadow/DiffuseMaterial_Shadow.h"


Scene_HelpMenu::Scene_HelpMenu(): GameScene(L"Scene_HelpMenu"), m_pButton_Start(nullptr), m_pButton_Back(nullptr), m_pKeyboard(nullptr), m_pGamepad(nullptr)
{
}


Scene_HelpMenu::~Scene_HelpMenu()
{
}

void Scene_HelpMenu::Initialize(const GameContext& gameContext)
{
	// CAMERA
	auto camera = new FixedCamera();
	AddChild(camera);

	camera->GetTransform()->Translate(0.f, 50.f, -125.f);
	camera->GetTransform()->Rotate(20.f, 0.f, 0.f);

	SetActiveCamera(camera->GetComponent<CameraComponent>());

	//BUTTONS
	m_pButton_Start = new BaseButtonPrefab({ 1000,600 }, L"./GAME/Resources/Textures/Menu's/Button_Start.png");

	m_pButton_Start->SetButtonCallback([this]()
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"Scene_MainLevel");
	});

	AddChild(m_pButton_Start);

	m_pButton_Back = new BaseButtonPrefab({ 50,600 }, L"./GAME/Resources/Textures/Menu's/Button_Back.png");

	m_pButton_Back->SetButtonCallback([this]()
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"Scene_MainMenu");
	});

	AddChild(m_pButton_Back);

	//IMAGES
	m_pKeyboard = new GameObject();
	m_pKeyboard->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/Menu's/Help_Keyboard.png"));
	AddChild(m_pKeyboard);

	m_pKeyboard->GetTransform()->Translate(100, 0, 0);

	m_pGamepad = new GameObject();
	m_pGamepad->AddComponent(new SpriteComponent(L"./GAME/Resources/Textures/Menu's/Help_Gamepad.png"));
	AddChild(m_pGamepad);
	
	m_pGamepad->GetTransform()->Translate(650, 0, 0);

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

}

void Scene_HelpMenu::Update(const GameContext& gameContext)
{
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

	//Back Button
	if (gameContext.pInput->IsActionTriggered(0))
	{
		if (mousePos.x > m_pButton_Back->GetPosition().x &&
			mousePos.x < m_pButton_Back->GetPosition().x + m_pButton_Back->GetWidth() &&
			mousePos.y > m_pButton_Back->GetPosition().y &&
			mousePos.y < m_pButton_Back->GetPosition().y + m_pButton_Back->GetHeight())
		{
			m_pButton_Back->OnClick();
		}
	}


	if (mousePos.x > m_pButton_Back->GetPosition().x &&
		mousePos.x < m_pButton_Back->GetPosition().x + m_pButton_Back->GetWidth() &&
		mousePos.y > m_pButton_Back->GetPosition().y &&
		mousePos.y < m_pButton_Back->GetPosition().y + m_pButton_Back->GetHeight())
	{
		m_pButton_Back->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Inverted_Back.png");
	}
	else
	{
		m_pButton_Back->SetTexture(L"./GAME/Resources/Textures/Menu's/Button_Back.png");
	}
}

void Scene_HelpMenu::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
