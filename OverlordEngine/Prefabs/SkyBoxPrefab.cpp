#include "stdafx.h"
#include "SkyBoxPrefab.h"
#include "../Components/ModelComponent.h"
#include "../../OverlordProject/Materials/SkyBoxMaterial.h"

SkyBoxPrefab::SkyBoxPrefab()
{
}


SkyBoxPrefab::~SkyBoxPrefab()
{
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext)
{
	auto modelcomp = new ModelComponent(L"./Resources/Meshes/Box.ovm");
	auto skyboxMat = new SkyBoxMaterial();
	skyboxMat->SetCubeMapTexture(L"./GAME/Resources/Textures/Skybox_Cubemap.dds");
	
	gameContext.pMaterialManager->AddMaterial(skyboxMat, 20);
	modelcomp->SetMaterial(20);
	AddComponent(modelcomp);
}

void SkyBoxPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyBoxPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
