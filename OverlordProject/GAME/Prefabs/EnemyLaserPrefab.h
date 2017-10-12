#pragma once
#include "Scenegraph/GameObject.h"

class RigidBodyComponent;
class Voxelizer_Explode;

class EnemyLaserPrefab : public GameObject
{
public:
	EnemyLaserPrefab(const XMFLOAT3 &direction, const float &speed, const int &matID);
	~EnemyLaserPrefab();

	bool GetActive() const { return m_Active; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	RigidBodyComponent *m_pRigid;
	Voxelizer_Explode *m_pExplodeMat;

	XMFLOAT3 m_Direction;
	float m_Speed, m_ExplodeTimer;
	bool m_Active, m_Explode;
	int m_MatID;

private:
};

