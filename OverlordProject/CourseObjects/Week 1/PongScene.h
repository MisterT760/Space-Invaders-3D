#pragma once
#include "Scenegraph/GameScene.h"

class SpherePrefab;
class CubePrefab;

class PongScene : public GameScene
{
public:
	PongScene();
	~PongScene();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	CubePrefab *m_Paddle1 = nullptr, *m_Paddle2 = nullptr;
	GameObject *m_TriggerLeft = nullptr, *m_TriggerRight = nullptr;
	SpherePrefab *m_Ball = nullptr;
	float m_PaddlePos1 = 0.0f, m_PaddlePos2 = 0.0f, m_Speed = 30.f, m_Elapsed = 0.f;
	int m_ScoreRed = 0, m_ScoreGreen = 0;
	bool m_Reset = false;

private:
	PongScene(const PongScene& other) = delete;
	PongScene& operator=(const PongScene& other) = delete;
};

