#pragma once
#include "Scenegraph/GameObject.h"

class MeshMaterial;
class Voxelizer_Disappear;

class BlockPrefab : public GameObject
{
public:
	BlockPrefab(XMFLOAT3 spawnPoint, int materialID, int disappearMat);
	~BlockPrefab();

	void Damage();
	bool GetActive() const { return m_Active; }
	int GetMaterialID() const { return m_MaterialID; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_Health = 250, m_DisappearTimer;
	bool m_Active, m_Disappear;
	XMFLOAT3 m_SpawnPoint;
	int m_MaterialID, m_DisappearMatID;
	MeshMaterial* m_pMat;
	Voxelizer_Disappear* m_pDisappearMat;
};

