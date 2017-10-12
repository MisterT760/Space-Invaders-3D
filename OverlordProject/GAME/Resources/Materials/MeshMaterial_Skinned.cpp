#include "stdafx.h"
#include "MeshMaterial_Skinned.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"


ID3DX11EffectVectorVariable* MeshMaterial_Skinned::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectShaderResourceVariable* MeshMaterial_Skinned::m_pDiffuseSRVVariable = nullptr;
ID3DX11EffectScalarVariable* MeshMaterial_Skinned::m_pInverted = nullptr;
ID3DX11EffectMatrixVariable* MeshMaterial_Skinned::m_pBoneTransformsVariable = nullptr;
ID3DX11EffectShaderResourceVariable* MeshMaterial_Skinned::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* MeshMaterial_Skinned::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* MeshMaterial_Skinned::m_pLightViewProjectionMatrix = nullptr;

MeshMaterial_Skinned::MeshMaterial_Skinned(): Material(L"./GAME/Resources/Effects/Game_Skinned.fx"), m_DiffuseTexture(nullptr)
{
}


MeshMaterial_Skinned::~MeshMaterial_Skinned()
{
}

void MeshMaterial_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_DiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void MeshMaterial_Skinned::LoadEffectVariables()
{
	m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gDiffuseColor")->AsVector();
	m_pDiffuseSRVVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_pInverted = m_pEffect->GetVariableByName("gInvert")->AsScalar();

	if (!m_pBoneTransformsVariable)
	{
		m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransformsVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransformsVariable = nullptr;
		}
	}

	if (!m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowSRVvariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gShadowMap\' variable not found!");
			m_pShadowSRVvariable = nullptr;
		}
	}

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pLightViewProjectionMatrix)
	{
		m_pLightViewProjectionMatrix = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();
		if (!m_pLightViewProjectionMatrix->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gLightViewProj\' variable not found!");
			m_pLightViewProjectionMatrix = nullptr;
		}
	}

}

void MeshMaterial_Skinned::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	m_pDiffuseColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_DiffuseColor));
	m_pDiffuseSRVVariable->SetResource(m_DiffuseTexture->GetShaderResourceView());
	m_pInverted->SetBool(m_Inverted);

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
