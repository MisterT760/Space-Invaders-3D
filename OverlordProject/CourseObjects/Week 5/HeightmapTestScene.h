#pragma once
#include "Scenegraph/GameScene.h"

class HeightmapTestScene : public GameScene
{
public:
	HeightmapTestScene();
	~HeightmapTestScene();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
};

