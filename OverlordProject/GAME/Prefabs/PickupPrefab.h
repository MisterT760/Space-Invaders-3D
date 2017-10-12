#pragma once
#include "Scenegraph/GameObject.h"

class PickupPrefab : public GameObject
{
public:

	enum PickupType : UINT
	{
		HEALTH = 0,
		SHIELD,
		REBUILD,
		RAPIDFIRE
	};

	PickupPrefab(XMFLOAT3 spawnPoint, PickupType type);
	~PickupPrefab();

	void Activate();
	PickupType GetType() const { return m_Type; }
	bool GetRebuild() const { return m_BlockRebuilt; }
	void SetRebuild(bool rebuild) { m_BlockRebuilt = rebuild; }
	bool GetActive() const { return m_Active; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	XMFLOAT3 m_pSpawnPoint;
	PickupType m_Type;
	bool m_Active = true, m_BlockRebuilt = false;
	float m_Rotation = 0, m_Speed = 15.f, m_LifeTime = 2.5f, m_Timer = 0.f;
};

