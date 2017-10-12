#pragma once
#include "Scenegraph/GameScene.h"

class BaseButtonPrefab;

class Scene_HelpMenu : public GameScene
{
public:
	Scene_HelpMenu();
	~Scene_HelpMenu();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	BaseButtonPrefab* m_pButton_Start, *m_pButton_Back;
	GameObject* m_pKeyboard, *m_pGamepad;
};

