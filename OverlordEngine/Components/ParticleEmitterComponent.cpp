//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount) :
	m_ParticleCount(particleCount),
	m_AssetFile(assetFile)
{
	while (int(m_Particles.size()) < m_ParticleCount)
	{
		m_Particles.push_back(new Particle(m_Settings));
	}
}


ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	for (size_t i = 0; i < m_Particles.size(); i++)
	{
		delete m_Particles[i];
	}
	m_Particles.clear();

	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWvpVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitter::LoadEffect() > Shader variable \'gWorldViewProj\' not valid!");
		return;
	}

	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();
	if (!m_pWvpVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitter::LoadEffect() > Shader variable \'gViewInverse\' not valid!");
		return;
	}

	m_pTextureVariable = m_pEffect->GetVariableByName("gParticleTexture")->AsShaderResource();
	if (!m_pWvpVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitter::LoadEffect() > Shader variable \'gParticleTexture\' not valid!");
		return;
	}

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	SafeRelease(m_pVertexBuffer);
	
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;
	
	auto result = gameContext.pDevice->CreateBuffer(&buffDesc, nullptr, &m_pVertexBuffer);
	Logger::LogHResult(result, L"ParticleEmitterComp->CreateVertexBuffer");
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	//See Lab10_2

	//BUFFER MAPPING CODE [PARTIAL :)]
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//ParticleVertex* pBuffer = (ParticleVertex*) mappedResource.pData;

	float particleInterval = ((m_Settings.MinEnergy + m_Settings.MaxEnergy) / 2.f) / m_Particles.size();
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ParticleVertex* pBuffer = (ParticleVertex*) mappedResource.pData;

	for	(auto particle : m_Particles)
	{
		particle->Update(gameContext);
		if (particle->IsActive())
		{
			pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
			m_ActiveParticles += 1;
		}
		else if (m_LastParticleInit >= particleInterval)
		{
			particle->Init(this->GetTransform()->GetWorldPosition());
			pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
			m_ActiveParticles += 1;
			m_LastParticleInit = 0;
		}
	}

	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	m_pWvpVariable->SetMatrix(&gameContext.pCamera->GetViewProjection()._11);
	m_pViewInverseVariable->SetMatrix(&gameContext.pCamera->GetViewInverse()._11);
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT j = 0; j < techDesc.Passes; ++j)
	{
		m_pDefaultTechnique->GetPassByIndex(j)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
