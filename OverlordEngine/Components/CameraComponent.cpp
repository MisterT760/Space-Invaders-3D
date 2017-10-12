//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CameraComponent.h"
#include "../Base/OverlordGame.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Base/GeneralStructs.h"


CameraComponent::CameraComponent(void):
	m_FOV(XM_PIDIV4),
	m_NearPlane(0.1f),
	m_FarPlane(2500.0f),
	m_Size(25.0f),
	m_PerspectiveProjection(true),
	m_IsActive(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view, viewInv, viewProjectionInv;

	if(m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio ,m_NearPlane, m_FarPlane);
	}
	else
	{
		float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	viewInv = XMMatrixInverse(nullptr, view);
	viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if(gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if(gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	auto mouseCoord = gameContext.pInput->GetMousePosition();
	XMFLOAT2 mouseNDC;

	auto width = OverlordGame::GetGameSettings().Window.Width / 2.f;
	auto height = OverlordGame::GetGameSettings().Window.Height / 2.f;

	mouseNDC.x = (static_cast<float>(mouseCoord.x) - width) / width;
	mouseNDC.y = (height - static_cast<float>(mouseCoord.y)) / height;

	XMFLOAT3 nearPoint, farPoint;

	XMVECTOR nearVec = XMVectorSet(mouseNDC.x, mouseNDC.y, 0, 0);
	XMVECTOR farVec = XMVectorSet(mouseNDC.x, mouseNDC.y, 1, 0);

	XMStoreFloat3(&nearPoint, XMVector3TransformCoord(nearVec, XMLoadFloat4x4(&GetViewProjectionInverse())));
	XMStoreFloat3(&farPoint, XMVector3TransformCoord(farVec, XMLoadFloat4x4(&GetViewProjectionInverse())));

	auto rayStart = PxVec3(nearPoint.x, nearPoint.y, nearPoint.z);

	auto direction = XMLoadFloat3(&farPoint) - XMLoadFloat3(&nearPoint);

	auto rayDir = PxVec3(XMVectorGetX(direction), XMVectorGetY(direction), XMVectorGetZ(direction));
	rayDir.normalize();

	PxQueryFilterData filterData;
	filterData.data.word0 = ~ignoreGroups;

	auto physXProxy = GetGameObject()->GetScene()->GetPhysxProxy();
	if (physXProxy == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to Pick. PhysX Proxy is null");
		return nullptr;
	}

	PxRaycastBuffer hit;
	if (physXProxy->Raycast(rayStart, rayDir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		Logger::LogInfo(L"Raycast Hit", false);
		auto object = reinterpret_cast<BaseComponent*>(hit.getAnyHit(hit.nbTouches).actor->userData);
		return object->GetGameObject();
	}

	return nullptr;
}