#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "RenderTarget.h"
#include "ShadowMapMaterial.h"
#include "OverlordGame.h"
#include "../Scenegraph/SceneManager.h"
#include "MeshFilter.h"
#include "../Components/ModelComponent.h"

ShadowMapRenderer::ShadowMapRenderer() :
m_IsInitialized(false),
m_LightDirection(1,0,0),
m_LightPosition(0,0,0),
m_pShadowMapMat(nullptr),
m_pShadowRT(nullptr),
m_Size(150)
{}


ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowMapMat);
	SafeDelete(m_pShadowRT);
}

void ShadowMapRenderer::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_IsInitialized) return;
	m_pShadowRT = new RenderTarget(gameContext.pDevice);

	RENDERTARGET_DESC desc{};
	desc.EnableColorBuffer = false;
	desc.EnableColorSRV = false;
	desc.EnableDepthBuffer = true;
	desc.EnableDepthSRV = true;

	desc.Width = OverlordGame::GetGameSettings().Window.Width;
	desc.Height = OverlordGame::GetGameSettings().Window.Height;

	desc.DepthFormat = DXGI_FORMAT_D32_FLOAT;
	desc.GenerateMipMaps_Color = false;

	auto hr = m_pShadowRT->Create(desc);
	Logger::LogHResult(hr, L"ShadowMapRenderer::Initialize() -> Failed to create the RenderTarget");
	m_pShadowMapMat = new ShadowMapMaterial(L"./Resources/Effects/Shadow/ShadowMap.fx");
	m_pShadowMapMat->Initialize(gameContext);
	
	m_IsInitialized = true;
}

void ShadowMapRenderer::Begin(const GameContext& gameContext)
{
	if (!m_IsInitialized)
	{
		Logger::LogError(L"ShadowMapRenderer::Begin() -> not initialized");
		return;
	}

	ID3D11ShaderResourceView* const pSRV[] = { nullptr };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	m_pShadowRT->Clear(gameContext, Colors::Red);

	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);

	auto window = OverlordGame::GetGameSettings().Window;

	float viewWidth = (m_Size > 0) ? m_Size * window.AspectRatio : window.Width;
	float viewHeight = (m_Size > 0) ? m_Size * window.AspectRatio : window.Height;

	XMMATRIX projMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 0.0f, 1000.0f);

	XMVECTOR worldPos = XMLoadFloat3(&m_LightPosition);
	XMVECTOR lookat = XMLoadFloat3(&m_LightDirection);
	XMVECTOR up = { 0, 1.0f, 0 };

	XMMATRIX viewMatrix = XMMatrixLookAtLH(worldPos, worldPos + lookat, up);
	XMStoreFloat4x4(&m_LightVP, viewMatrix * projMatrix);

	m_pShadowMapMat->SetLightVP(m_LightVP);
}

void ShadowMapRenderer::Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, ModelComponent* pModelComponent, XMFLOAT4X4 world)
{
	m_pShadowMapMat->SetWorld(world);
	if (pModelComponent->HasAnimator())
	{
		m_pShadowMapMat->EnableBones(true);
	}
	else
	{
		m_pShadowMapMat->EnableBones(false);
	}
	m_pShadowMapMat->UpdateEffectVariables(gameContext, pModelComponent);

	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMapMat->GetInputLayout());

	//VertexBuffer
	UINT offset = 0;
	auto vBufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMapMat);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vBufferData.pVertexBuffer, &vBufferData.VertexStride, &offset);

	//IndexBuffer
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw
	auto technique = m_pShadowMapMat->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	technique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		technique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::End(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_IsInitialized) SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowSRV() const
{
	return m_pShadowRT->GetDepthShaderResourceView();
}

void ShadowMapRenderer::SetLight(const XMFLOAT3& position, const XMFLOAT3& direction)
{
	m_LightPosition = position;
	m_LightDirection = direction;


	/*XMStoreFloat4x4(&m_LightVP, XMMatrixIdentity());
	auto window = OverlordGame::GetGameSettings().Window;

	float width, height;
	if (m_Size > 0.0f)
	{
		width = m_Size * window.AspectRatio;
		height = m_Size * window.AspectRatio;
	}
	else
	{
		width = static_cast<float>(window.Width);
		height = static_cast<float>(window.Height);
	}

	auto projection = XMMatrixOrthographicLH(width, height, 0.0f, 1000.0f);
	
	auto worldPos = XMLoadFloat3(&m_LightPosition);
	auto lookAt = XMLoadFloat3(&m_LightDirection);
	auto upV = XMFLOAT3(0.0f, 1.0f, 0.0f);
	auto up = XMLoadFloat3(&upV);

	auto view = XMMatrixLookAtLH(worldPos, worldPos + lookAt, up);
	XMStoreFloat4x4(&m_LightVP, view * projection);*/
}
