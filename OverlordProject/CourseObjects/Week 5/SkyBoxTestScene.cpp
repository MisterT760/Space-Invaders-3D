#include "stdafx.h"
#include "SkyBoxTestScene.h"
#include "Prefabs/SkyBoxPrefab.h"


SkyBoxTestScene::SkyBoxTestScene() : GameScene(L"SkyBoxTestScene")
{
}


SkyBoxTestScene::~SkyBoxTestScene()
{
}

void SkyBoxTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto skyBox = new SkyBoxPrefab;
	AddChild(skyBox);
}

void SkyBoxTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyBoxTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
