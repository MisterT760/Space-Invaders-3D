#pragma once
#include "BaseComponent.h"

class ControllerComponent : public BaseComponent
{
public:
	ControllerComponent(PxMaterial* material, XMFLOAT3 updirection = XMFLOAT3(0, 1, 0), float radius = 5, float height = 15, wstring name = L"Character", PxCapsuleClimbingMode::Enum climbingMode = PxCapsuleClimbingMode::eEASY);
	virtual ~ControllerComponent();

	void Translate(XMFLOAT3 position);
	void Translate(float x, float y, float z);
	void Move(XMFLOAT3 displacement, float minDist = 0);
	const PxControllerCollisionFlags& GetCollisionFlags() const { return m_CollisionFlag; }
	void SetCollisionGroup(CollisionGroupFlag group);
	void SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetFootPosition();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_Radius, m_Height;
	XMFLOAT3 m_UpDirection;
	wstring m_Name;
	PxController* m_pController;
	PxCapsuleClimbingMode::Enum m_ClimbingMode;
	PxMaterial* m_pMaterial;
	PxControllerCollisionFlags m_CollisionFlag;
	PxFilterData m_CollisionGroups;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ControllerComponent( const ControllerComponent& obj);
	ControllerComponent& operator=( const ControllerComponent& obj);
};

