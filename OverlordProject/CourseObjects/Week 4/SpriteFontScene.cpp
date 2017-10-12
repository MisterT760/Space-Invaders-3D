#include "stdafx.h"
#include "SpriteFontScene.h"
#include "Graphics/TextRenderer.h"
#include "Content/ContentManager.h"


SpriteFontScene::SpriteFontScene(void) :
	GameScene(L"SpriteFontScene")
{
}


SpriteFontScene::~SpriteFontScene(void)
{
}

void SpriteFontScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/Font/Calibri_32.fnt");
}

void SpriteFontScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SpriteFontScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	TextRenderer::GetInstance()->DrawText(m_pFont, L"Hello World!", { 10.f, 10.f } , XMFLOAT4(Colors::Black));
}
