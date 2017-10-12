#pragma once
#include "BaseEnemy.h"

class EnemyController;
class EnemyLaserPrefab;
class ModelComponent;
class Voxelizer_Fall;

class SmallInvader : public BaseEnemy
{
public:
	SmallInvader(const XMFLOAT3 spawnPoint, const float x_Offset, const float z_Offset, const float speed, const int matID, const int laserMatID);
	~SmallInvader();

	EnemyLaserPrefab* Attack(const GameContext& gameContext) override;
	void Movement(const GameContext& gameContext) override;
	void SetMoveZ(const bool moveZ) { m_MoveZ = moveZ; }
	void SetMoveSpeed(const float speed) { m_Speed = speed; }
	void SetShootInterval(const float interval) { m_Interval = interval; }
	void SetShootSpeed(const float speed) { m_ShootSpeed = speed; }
	XMFLOAT3 GetControllerPosition() const;
	void Damage() override;

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	float m_Timer = 0.0f, m_Size = 5.0f, m_X_BaseOffset = 10.f, m_Z_BaseOffset = 11.5f, m_X_Offset, m_Z_Offset, m_X_Direction = 1.f, m_Speed = 20.f, m_Interval = 5.f, m_ShootSpeed = 150.f, m_FallTime = 0.f;
	int m_StartTime = 0, m_IntervalOffset = 0, m_LaserMatID, m_MatID;
	XMFLOAT3 m_Direction = { 1,0,0 };
	EnemyController* m_pController = nullptr;
	bool m_MoveZ = true, m_ShouldAnimate = false, m_Fall = false;
	ModelComponent* m_pModelComponent = nullptr;
	Voxelizer_Fall* m_pFallMat;
};

