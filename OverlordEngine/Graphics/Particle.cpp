//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings) :
	m_EmitterSettings(emitterSettings)
{
}


Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive)
	{
		return;
	}

	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy <= 0)
	{
		m_IsActive = false;
		return;
	}

	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;

	XMFLOAT3 totalPos;
	XMStoreFloat3(&totalPos, XMLoadFloat3(&m_VertexInfo.Position) + (XMLoadFloat3(&m_EmitterSettings.Velocity) * gameContext.pGameTime->GetElapsed()));
	m_VertexInfo.Position = totalPos;
	m_VertexInfo.Color = m_EmitterSettings.Color;
	m_VertexInfo.Color.w = particleLifePercent;

	if (m_SizeGrow < 1)
	{
		m_VertexInfo.Size = m_InitSize + (m_SizeGrow * particleLifePercent);
	}
	else if (m_SizeGrow > 1)
	{
		m_VertexInfo.Size = m_InitSize + ((m_InitSize * m_SizeGrow - m_InitSize) * (1 - particleLifePercent));
	}
}

void Particle::Init(XMFLOAT3 initPosition)
{
	m_IsActive = true;

	auto energy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_TotalEnergy = energy;
	m_CurrentEnergy = energy;

	auto randomDirection = XMFLOAT3(1.f, 0, 0);
	auto randomRotationMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	auto randNormVec = XMVector3TransformNormal(XMLoadFloat3(&randomDirection), randomRotationMatrix);
	auto emitterRange = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);

	XMStoreFloat3(&m_VertexInfo.Position, XMLoadFloat3(&initPosition) + (randNormVec * emitterRange));

	auto size = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_InitSize = size;
	m_VertexInfo.Size = size;
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);

	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
