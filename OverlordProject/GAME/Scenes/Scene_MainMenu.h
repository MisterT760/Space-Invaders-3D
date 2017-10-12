#pragma once
#include "Scenegraph/GameScene.h"

class BaseButtonPrefab;
class GameObject;
class PostCA;

class Scene_MainMenu : public GameScene
{
public:
	Scene_MainMenu();
	~Scene_MainMenu();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	BaseButtonPrefab* m_pButton_Start, *m_pButton_Help, *m_pButton_Exit;
	GameObject* m_pShip, *m_pGameName;
	PostCA* m_pChromaticAbboration = nullptr;
	int m_Time = 0;
	float m_Timer = 0.0f, m_CATimer = 0.0f, m_CATime = 0.0f;
	bool m_BlurGray = false;
};

