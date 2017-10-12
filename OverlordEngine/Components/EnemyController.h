#pragma once
#include "BaseComponent.h"

class EnemyController : public BaseComponent
{
public:
	EnemyController(PxMaterial* material, XMFLOAT3 updirection, float radius, wstring name);
	~EnemyController();

	void Translate(XMFLOAT3 position);
	void Translate(float x, float y, float z);
	void Move(XMFLOAT3 displacement, float minDist = 0);
	const PxControllerCollisionFlags& GetCollisionFlags() const { return m_CollisionFlag; }
	void SetCollisionGroup(CollisionGroupFlag group);
	void SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetFootPosition();


protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:

	float m_Radius;
	XMFLOAT3 m_UpDirection;
	wstring m_Name;
	PxController* m_pController;
	PxMaterial* m_pMaterial;
	PxControllerCollisionFlags m_CollisionFlag;
	PxFilterData m_CollisionGroups;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	EnemyController(const EnemyController& obj) = delete;
	EnemyController& operator=(const EnemyController& obj) = delete;
};

