#include "stdafx.h"
#include "BaseButtonPrefab.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"
#include "Components/Components.h"

BaseButtonPrefab::BaseButtonPrefab(XMFLOAT2 position, wstring imagePath) : m_Width(0), m_Height(0), m_Position(position), m_ImagePath(imagePath)
{
}


BaseButtonPrefab::~BaseButtonPrefab()
{
}

void BaseButtonPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto image = ContentManager::Load<TextureData>(m_ImagePath);

	m_Width = image->GetDimension().x;
	m_Height = image->GetDimension().y;

	m_Image = new SpriteComponent(m_ImagePath, XMFLOAT2(0.f, 0.f));
	AddComponent(m_Image);

	GetTransform()->Translate(m_Position.x, m_Position.y, 0);
}

void BaseButtonPrefab::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BaseButtonPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BaseButtonPrefab::SetButtonCallback(function<void()> callback)
{
	m_Callback = callback;
}

void BaseButtonPrefab::SetTexture(wstring textureFile)
{
	m_Image->SetTexture(textureFile);
}

void BaseButtonPrefab::OnClick() const
{
	if (m_Callback)
		m_Callback();
}
