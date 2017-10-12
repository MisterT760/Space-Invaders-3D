#include "stdafx.h"
#include "EnemyController.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Physx/PhysxProxy.h"
#include "Components.h"


EnemyController::EnemyController(PxMaterial* material, XMFLOAT3 updirection, float radius, wstring name)
	: m_Radius(radius),
	m_UpDirection(updirection),
	m_Name(name),
	m_pController(nullptr),
	m_pMaterial(material),
	m_CollisionFlag(PxControllerCollisionFlags()),
	m_CollisionGroups(PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0))
{
}


EnemyController::~EnemyController()
{
	m_pController->release();
}

void EnemyController::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_pController != nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot initialize a controller twice");

	//1. Retrieve the ControllerManager from the PhysX Proxy (PhysxProxy::GetControllerManager();)
	auto controllerManager = GetGameObject()->GetScene()->GetPhysxProxy()->GetControllerManager();
	//2. Create a PxCapsuleControllerDesc (Struct)
	//  > Call the "setToDefault()" method of the PxCapsuleControllerDesc
	//	> Fill in all the required fields
	//  > Radius, Height, ClimbingMode, UpDirection (PxVec3(0,1,0)), ContactOffset (0.1f), Material [See Initializer List]
	//  > Position -> Use the position of the parent GameObject
	//  > UserData -> This component
	auto controlDesc = PxBoxControllerDesc();
	controlDesc.setToDefault();
	controlDesc.halfSideExtent = m_Radius;
	controlDesc.halfForwardExtent = m_Radius;
	controlDesc.halfHeight = m_Radius;
	controlDesc.upDirection = ToPxVec3(m_UpDirection);
	controlDesc.contactOffset = 0.1f;
	controlDesc.material = m_pMaterial;
	controlDesc.position = PxExtendedVec3(GetGameObject()->GetTransform()->GetPosition().x, GetGameObject()->GetTransform()->GetPosition().y, GetGameObject()->GetTransform()->GetPosition().z);
	controlDesc.userData = this;
	//3. Create the controller object (m_pController), use the ControllerManager to do that (CHECK IF VALID!!)
	if (!controlDesc.isValid())
	{
		Logger::LogError(L"[ControllerComponent] Failed! ControllerDescriptor is Invalid!");
		return;
	}

	m_pController = controllerManager->createController(controlDesc);

	if (m_pController == nullptr)
	{
		Logger::LogError(L"[ControllerComponent] Failed to create controller");
		return;
	}

	//4. Set the controller's name (use the value of m_Name) [PxController::setName]
	//   > Converting 'wstring' to 'string' > Use one of the constructor's of the string class
	//	 > Converting 'string' to 'char *' > Use one of the string's methods ;)
	m_pController->getActor()->setName(string(m_Name.begin(), m_Name.end()).c_str());
	//5. Set the controller's actor's userdata > This Component
	m_pController->getActor()->userData = this;

	SetCollisionGroup((CollisionGroupFlag)m_CollisionGroups.word0);
	SetCollisionIgnoreGroups((CollisionGroupFlag)m_CollisionGroups.word1);
}

void EnemyController::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void EnemyController::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void EnemyController::Translate(XMFLOAT3 position)
{
	if (m_pController == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Translate. No controller present");
	else
		m_pController->setPosition(ToPxExtendedVec3(position));
}

void EnemyController::Translate(float x, float y, float z)
{
	Translate(XMFLOAT3(x, y, z));
}

void EnemyController::Move(XMFLOAT3 displacement, float minDist)
{
	if (m_pController == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Move. No controller present");
	else
		m_CollisionFlag = m_pController->move(ToPxVec3(displacement), minDist, 0, 0, 0);
}

void EnemyController::SetCollisionGroup(CollisionGroupFlag group)
{
	m_CollisionGroups.word0 = group;

	if (!m_pController)
		return;

	auto nbShapes = m_pController->getActor()->getNbShapes();

	PxShape* buffer;
	m_pController->getActor()->getShapes(&buffer, nbShapes * sizeof(PxShape));

	for (UINT i = 0; i < nbShapes; ++i)
	{
		buffer[i].setSimulationFilterData(m_CollisionGroups);
		buffer[i].setQueryFilterData(m_CollisionGroups);
	}
}

void EnemyController::SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups)
{
	m_CollisionGroups.word1 = ignoreGroups;

	if (!m_pController)
		return;

	auto nbShapes = m_pController->getActor()->getNbShapes();

	PxShape* buffer;
	m_pController->getActor()->getShapes(&buffer, nbShapes * sizeof(PxShape));

	for (UINT i = 0; i < nbShapes; ++i)
	{
		buffer[i].setSimulationFilterData(m_CollisionGroups);
	}
}

XMFLOAT3 EnemyController::GetPosition()
{
	if (m_pController == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get position. No controller present");
	else
		return ToXMFLOAT3(m_pController->getPosition());

	return XMFLOAT3();
}

XMFLOAT3 EnemyController::GetFootPosition()
{
	if (m_pController == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get footposition. No controller present");
	else
		return ToXMFLOAT3(m_pController->getFootPosition());

	return XMFLOAT3();
}
