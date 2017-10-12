//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedDiffuseMaterial_Shadow.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pBoneTransformsVariable = nullptr;
ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* SkinnedDiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pLightViewProjectionMatrix = nullptr;

SkinnedDiffuseMaterial_Shadow::SkinnedDiffuseMaterial_Shadow() : 
	Material(L"./Resources/Effects/Shadow/PosNormTex3D_Skinned_Shadow.fx"),
	m_pDiffuseTexture(nullptr)
{
}


SkinnedDiffuseMaterial_Shadow::~SkinnedDiffuseMaterial_Shadow()
{

}

void SkinnedDiffuseMaterial_Shadow::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial_Shadow::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gShadowMap\' variable not found!");
			m_pShadowSRVvariable = nullptr;
		}
	}

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pLightViewProjectionMatrix)
	{
		m_pLightViewProjectionMatrix = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();
		if (!m_pLightViewProjectionMatrix->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightViewProj\' variable not found!");
			m_pLightViewProjectionMatrix = nullptr;
		}
	}

	if (!m_pBoneTransformsVariable)
	{
		m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransformsVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransformsVariable = nullptr;
		}
	}
}

void SkinnedDiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	if (m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable->SetRawValue(&m_LightDirection, 0, sizeof(m_LightDirection));
	}

	if (m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowSRV());
	}

	if (m_pLightViewProjectionMatrix)
	{
		m_pLightViewProjectionMatrix->SetMatrix(reinterpret_cast<float*>(&gameContext.pShadowMapper->GetLightVP()));
	}

	if (m_pBoneTransformsVariable && pModelComponent->HasAnimator())
	{
		vector<XMFLOAT4X4> boneTransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
		float* arr[70];
		for (size_t i = 0; i < 70; i++)
		{
			if (i < boneTransforms.size()) arr[i] = reinterpret_cast<float*>(&boneTransforms[i]);
		}
		m_pBoneTransformsVariable->SetMatrixPointerArray(reinterpret_cast<const float**>(&arr), 0, boneTransforms.size());
	}
}
