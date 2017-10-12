#include "stdafx.h"
#include "MeshMaterial.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"


ID3DX11EffectVectorVariable* MeshMaterial::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectShaderResourceVariable* MeshMaterial::m_pDiffuseSRVVariable = nullptr;
ID3DX11EffectScalarVariable* MeshMaterial::m_pInverted = nullptr;
ID3DX11EffectShaderResourceVariable* MeshMaterial::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* MeshMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* MeshMaterial::m_pLightViewProjectionMatrix = nullptr;

MeshMaterial::MeshMaterial(): Material(L"./GAME/Resources/Effects/Game.fx"), m_DiffuseTexture(nullptr)
{
}


MeshMaterial::~MeshMaterial()
{
}

void MeshMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_DiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void MeshMaterial::LoadEffectVariables()
{
	m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gDiffuseColor")->AsVector();
	m_pDiffuseSRVVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_pInverted = m_pEffect->GetVariableByName("gInvert")->AsScalar();

	if (!m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gShadowMap\' variable not found!");
			m_pShadowSRVvariable = nullptr;
		}
	}

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pLightViewProjectionMatrix)
	{
		m_pLightViewProjectionMatrix = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();
		if (!m_pLightViewProjectionMatrix->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightViewProj\' variable not found!");
			m_pLightViewProjectionMatrix = nullptr;
		}
	}
}

void MeshMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(pModelComponent);

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
}
