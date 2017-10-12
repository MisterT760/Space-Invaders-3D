#include "stdafx.h"
#include "CharacterTest.h"
#include "Physx/PhysxManager.h"
#include "Physx/PhysxProxy.h"
#include "Components/Components.h"
#include "Scenegraph/GameObject.h"


CharacterTest::CharacterTest() : GameScene(L"CharacterTestScene")
{
}


CharacterTest::~CharacterTest()
{
}

void CharacterTest::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto bouncyMaterial = physX->createMaterial(0.f, 0.f, 0.10f);
	//auto characterMaterial = physX->createMaterial(0.f, 0.f, 0.1f);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CONTROLLER
	//**********
	auto character = new Character;
	AddChild(character);

	character->GetTransform()->Translate(0.f, 10.f, 0.f);

}

void CharacterTest::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
