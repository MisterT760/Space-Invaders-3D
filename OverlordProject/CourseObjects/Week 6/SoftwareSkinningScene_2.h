#pragma once
#include "Scenegraph/GameScene.h"
#include "BoneObject.h"
#include "Components/MeshDrawComponent.h"

class SoftwareSkinningScene_2 : public GameScene
{
public:
	SoftwareSkinningScene_2();
	~SoftwareSkinningScene_2();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:
	void CreateMesh(float lenght);

	BoneObject *m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;
	MeshDrawComponent* m_pMeshDrawer;
	vector<SkinnedVertex> m_SkinnedVertices;
};

