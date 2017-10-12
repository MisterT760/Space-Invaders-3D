#include "stdafx.h"
#include "ShadowMapMaterial.h"
#include "../Components/ModelComponent.h"
#include "ModelAnimator.h"

ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pLightVPMatrixVariable = nullptr;
ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pWorldMatrixVariable = nullptr;
ID3DX11EffectScalarVariable* ShadowMapMaterial::m_pHasBonesVariable = nullptr;
ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pBoneTransformsVariable = nullptr;

ShadowMapMaterial::ShadowMapMaterial(const wstring& effectFile, const wstring& technique):
	Material(effectFile, technique), m_HasBones(false)
{
}


ShadowMapMaterial::~ShadowMapMaterial()
{
}

void ShadowMapMaterial::SetLightVP(XMFLOAT4X4 lightVP)
{
	m_LightVP = lightVP;
}

void ShadowMapMaterial::SetWorld(XMFLOAT4X4 world)
{
	m_World = world;
}

void ShadowMapMaterial::EnableBones(bool enable)
{
	m_HasBones = enable;
}

void ShadowMapMaterial::LoadEffectVariables()
{
	if (!m_pLightVPMatrixVariable)
	{
		m_pLightVPMatrixVariable = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();
		if (!m_pLightVPMatrixVariable->IsValid())
		{
			Logger::LogWarning(L"ShadowMapMaterial::LoadEffectVariables() > \'gLightViewProj\' variable not found!");
			m_pLightVPMatrixVariable = nullptr;
		}
	}

	if (!m_pWorldMatrixVariable)
	{
		m_pWorldMatrixVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
		if (!m_pWorldMatrixVariable->IsValid())
		{
			Logger::LogWarning(L"ShadowMapMaterial::LoadEffectVariables() > \'gWorld\' variable not found!");
			m_pWorldMatrixVariable = nullptr;
		}
	}

	if (!m_pBoneTransformsVariable)
	{
		m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransformsVariable->IsValid())
		{
			Logger::LogWarning(L"ShadowMapMaterial::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransformsVariable = nullptr;
		}
	}

	if (!m_pHasBonesVariable)
	{
		m_pHasBonesVariable = m_pEffect->GetVariableByName("gHasBones")->AsScalar();
		if (!m_pHasBonesVariable->IsValid())
		{
			Logger::LogWarning(L"ShadowMapMaterial::LoadEffectVariables() > \'gHasBones\' variable not found!");
			m_pHasBonesVariable = nullptr;
		}
	}
}

void ShadowMapMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_HasBones)
	{
		vector<XMFLOAT4X4> bonetransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
		m_pBoneTransformsVariable->SetMatrixArray(reinterpret_cast<float*>(&bonetransforms[0]), 0, bonetransforms.size());
	}
	m_pHasBonesVariable->SetBool(m_HasBones);

	m_pWorldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_World));
	m_pLightVPMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_LightVP));

	m_pLightVPMatrixVariable->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&m_LightVP)));
	m_pWorldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&m_World)));
}
