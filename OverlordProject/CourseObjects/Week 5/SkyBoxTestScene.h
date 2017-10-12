#pragma once
#include "Scenegraph/GameScene.h"

class SkyBoxTestScene : public GameScene
{
public:
	SkyBoxTestScene();
	~SkyBoxTestScene();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:

};

