#pragma once
#include "Scenegraph/GameScene.h"
#include "BoneObject.h"

class SoftwareSkinningScene_1 : public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:
	BoneObject *m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;
};

