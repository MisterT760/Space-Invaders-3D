#include "stdafx.h"
#include "Scene_GeometryShader.h"
#include "Components/ModelComponent.h"
#include "Scenegraph/GameObject.h"
#include "Components/TransformComponent.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Explode.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Fall.h"
#include "../OverlordProject/GEOMETRY_SHADER/Material/Voxelizer_Disappear.h"
#include "Graphics/TextRenderer.h"
#include "Content/ContentManager.h"
#include "Diagnostics/DebugRenderer.h"


Scene_GeometryShader::Scene_GeometryShader(): GameScene(L"Scene_GeometryShader"), m_Timer(0), m_Activate(false), m_Rewind(false)
{
}


Scene_GeometryShader::~Scene_GeometryShader()
{
}

void Scene_GeometryShader::Initialize(const GameContext& gameContext)
{
	DebugRenderer::ToggleDebugRenderer();

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/Font/Calibri_32.fnt");
	m_EffectType = L"Explode";

	m_pModel = new GameObject();
	m_pExplodeMat = new Voxelizer_Explode();
	m_pExplodeMat->SetDiffuseColor(XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	m_pExplodeMat->SetCubeSize(m_CubeSize);
	m_pExplodeMat->SetExplosionForce(m_ExplosionForce);
	m_pExplodeMat->SetGravity(m_Gravity);
	m_pExplodeMat->SetTime(m_Timer);
	m_pExplodeMat->SetSlowMotion(m_SlowMotion);
	m_pExplodeMat->SetMaxTime(10.f);
	gameContext.pMaterialManager->AddMaterial(m_pExplodeMat, 50);

	m_pFallMat = new Voxelizer_Fall();
	m_pFallMat->SetDiffuseColor(XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	m_pFallMat->SetCubeSize(m_CubeSize);
	m_pFallMat->SetGravity(m_Gravity);
	m_pFallMat->SetTime(m_Timer);
	m_pFallMat->SetSlowMotion(m_SlowMotion);
	m_pFallMat->SetMaxTime(10.f);
	gameContext.pMaterialManager->AddMaterial(m_pFallMat, 51);

	m_pDisappearMat = new Voxelizer_Disappear();
	m_pDisappearMat->SetDiffuseColor(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	m_pDisappearMat->SetCubeSize(m_CubeSize);
	m_pDisappearMat->SetOpacityFactor(5.f);
	m_pDisappearMat->SetTime(m_Timer);
	gameContext.pMaterialManager->AddMaterial(m_pDisappearMat, 52);

	auto modelComponent = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	modelComponent->SetMaterial(50);
	m_pModel->AddComponent(modelComponent);

	AddChild(m_pModel);

	m_pModel->GetTransform()->Scale(1.0f, 1.0f, 1.0f);
	gameContext.pInput->AddInputAction(InputAction(0, Pressed, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(1, Pressed, 'R'));
	gameContext.pInput->AddInputAction(InputAction(2, Pressed, 'P'));
	gameContext.pInput->AddInputAction(InputAction(3, Pressed, 'E'));
	gameContext.pInput->AddInputAction(InputAction(4, Pressed, '1'));
	gameContext.pInput->AddInputAction(InputAction(5, Pressed, '2'));
	gameContext.pInput->AddInputAction(InputAction(6, Pressed, '3'));
	gameContext.pInput->AddInputAction(InputAction(7, Down, 'Q'));
	gameContext.pInput->AddInputAction(InputAction(8, Down, 'Z'));
	gameContext.pInput->AddInputAction(InputAction(9, Down, 'X'));
	gameContext.pInput->AddInputAction(InputAction(10, Down, 'C'));
	gameContext.pInput->AddInputAction(InputAction(11, Down, 'N'));
	gameContext.pInput->AddInputAction(InputAction(12, Down, 'M'));
	gameContext.pInput->AddInputAction(InputAction(13, Down, 'K'));
	gameContext.pInput->AddInputAction(InputAction(14, Down, 'L'));
	gameContext.pInput->AddInputAction(InputAction(15, Down, 'H'));
	gameContext.pInput->AddInputAction(InputAction(16, Down, 'J'));
}

void Scene_GeometryShader::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(0))
	{
		m_Activate = true;
		m_Rewind = false;
	}

	if (gameContext.pInput->IsActionTriggered(1))
	{
		m_Rewind = true;
		m_Activate = false;
	}

	if (gameContext.pInput->IsActionTriggered(2))
	{
		m_Rewind = false;
		m_Activate = false;
	}

	if (gameContext.pInput->IsActionTriggered(3))
	{
		m_Rewind = false;
		m_Activate = false;
		m_Timer = 0.f;
	}

	if (gameContext.pInput->IsActionTriggered(4))
	{
		m_Rewind = false;
		m_Activate = false;
		m_Timer = 0.f;
		m_pModel->GetComponent<ModelComponent>()->SetMaterial(50);
		m_EffectType = L"Explode";
	}

	if (gameContext.pInput->IsActionTriggered(5))
	{
		m_Rewind = false;
		m_Activate = false;
		m_Timer = 0.f;
		m_pModel->GetComponent<ModelComponent>()->SetMaterial(51);
		m_EffectType = L"Fall";
	}

	if (gameContext.pInput->IsActionTriggered(6))
	{
		m_Rewind = false;
		m_Activate = false;
		m_Timer = 0.f;
		m_pModel->GetComponent<ModelComponent>()->SetMaterial(52);
		m_EffectType = L"Disappear";
	}

	if (gameContext.pInput->IsActionTriggered(7))
	{
		m_CubeSize -= 0.001f;
	}

	if (gameContext.pInput->IsActionTriggered(8))
	{
		m_CubeSize += 0.001f;
	}

	if (gameContext.pInput->IsActionTriggered(9))
	{
		m_SlowMotion -= 0.01f;
	}

	if (gameContext.pInput->IsActionTriggered(10))
	{
		m_SlowMotion += 0.01f;
	}

	if (gameContext.pInput->IsActionTriggered(11))
	{
		m_ExplosionForce -= 1.f;
	}

	if (gameContext.pInput->IsActionTriggered(12))
	{
		m_ExplosionForce += 1.f;
	}

	if (gameContext.pInput->IsActionTriggered(13))
	{
		m_Gravity -= 0.1f;
	}

	if (gameContext.pInput->IsActionTriggered(14))
	{
		m_Gravity += 0.1f;
	}

	if (gameContext.pInput->IsActionTriggered(15))
	{
		m_OpacityFactor -= 0.1f;
	}

	if (gameContext.pInput->IsActionTriggered(16))
	{
		m_OpacityFactor += 0.1f;
	}

	if (m_Activate == true)
	{
		m_Timer += gameContext.pGameTime->GetElapsed();
	}

	if (m_Rewind == true)
	{
		m_Timer -= gameContext.pGameTime->GetElapsed();
		if (m_Timer < 0.f)
		{
			m_Timer = 0.f;
		}
	}

	m_pDisappearMat->SetOpacityFactor(m_OpacityFactor);

	m_pExplodeMat->SetExplosionForce(m_ExplosionForce);

	m_pExplodeMat->SetGravity(m_Gravity);
	m_pFallMat->SetGravity(m_Gravity);

	m_pExplodeMat->SetCubeSize(m_CubeSize);
	m_pFallMat->SetCubeSize(m_CubeSize);
	m_pDisappearMat->SetCubeSize(m_CubeSize);

	m_pExplodeMat->SetSlowMotion(m_SlowMotion);
	m_pFallMat->SetSlowMotion(m_SlowMotion);

	m_pExplodeMat->SetTime(m_Timer);
	m_pFallMat->SetTime(m_Timer);
	m_pDisappearMat->SetTime(m_Timer);
}

void Scene_GeometryShader::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	TextRenderer::GetInstance()->DrawText(m_pFont, L"Effect Type: " + m_EffectType, {0,0}, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press R to Rewind", { 0,25 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press E to Reset", { 0,50 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press Space to start the Timer " + std::to_wstring(m_Timer), { 0,75 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press P to Pause", { 0,100 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press 1, 2, 3 to switch effects", { 0,125 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press Q, Z to make cubes smaller/bigger " + std::to_wstring(m_CubeSize), { 0,150 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press X, C to slow time less/more " + std::to_wstring(m_SlowMotion), { 0,175 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press N, M to change the explosion force " + std::to_wstring(m_ExplosionForce), { 0,200 }, static_cast<XMFLOAT4>(Colors::White));
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Press K, L to change gravity " + std::to_wstring(m_Gravity), { 0,225 }, static_cast<XMFLOAT4>(Colors::White));
	if (m_EffectType == L"Disappear")
	{
		TextRenderer::GetInstance()->DrawText(m_pFont, L"Press H, J to change opacity factor " + std::to_wstring(m_OpacityFactor), { 0,250 }, static_cast<XMFLOAT4>(Colors::White));
	}
}
