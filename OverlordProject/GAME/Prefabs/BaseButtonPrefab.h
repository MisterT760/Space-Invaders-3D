#pragma once
#include "SceneGraph/GameObject.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

class BaseButtonPrefab : public GameObject
{
public:
	BaseButtonPrefab(XMFLOAT2 position, wstring imagePath);
	~BaseButtonPrefab();

	void SetButtonCallback(function<void(void)> callback);
	void SetTexture(wstring textureFile);
	void OnClick() const;
	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }
	XMFLOAT2 GetPosition() const { return m_Position; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_Width, m_Height;
	XMFLOAT2 m_Position;
	wstring m_ImagePath;
	SpriteComponent* m_Image = nullptr;
	function<void(void)> m_Callback;
};
