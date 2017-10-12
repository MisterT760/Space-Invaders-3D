#pragma once
#include "../Scenegraph/GameObject.h"

class SkyBoxPrefab : public GameObject
{
public:
	SkyBoxPrefab();
	~SkyBoxPrefab();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	SkyBoxPrefab(const SkyBoxPrefab &obj) = delete;
	SkyBoxPrefab& operator=(const SkyBoxPrefab& obj) = delete;
};

