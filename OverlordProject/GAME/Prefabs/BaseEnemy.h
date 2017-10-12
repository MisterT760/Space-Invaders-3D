#pragma once
#include "Scenegraph/GameObject.h"

class EnemyLaserPrefab;

class BaseEnemy : public GameObject
{
public:
	BaseEnemy(const XMFLOAT3 spawnPoint);
	~BaseEnemy();

	virtual void Damage();
	void SetMeshResource(wstring meshResource) { m_MeshResource = meshResource; }
	virtual void Movement(const GameContext& gameContext) = 0;
	bool GetActive() const { return m_Active; }
	void SetActive(bool active) { m_Active = active; }

protected:
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

	virtual EnemyLaserPrefab* Attack(const GameContext& gameContext) = 0;

	XMFLOAT3 m_Direction;
	float m_Health;
	bool m_Active;
	XMFLOAT3 m_SpawnPoint;
	wstring m_MeshResource;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BaseEnemy(const BaseEnemy &obj) = delete;
	BaseEnemy& operator=(const BaseEnemy& obj) = delete;
};

