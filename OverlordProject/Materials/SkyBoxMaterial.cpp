#include "stdafx.h"
#include "SkyBoxMaterial.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pCubeMapSRVVariable = nullptr;

SkyBoxMaterial::SkyBoxMaterial() : Material(L"./Resources/Effects/Skybox.fx"), m_CubeMap(nullptr)
{
}


SkyBoxMaterial::~SkyBoxMaterial()
{
}

void SkyBoxMaterial::SetCubeMapTexture(const std::wstring& assetFile)
{
	m_CubeMap = ContentManager::Load<TextureData>(assetFile);
}

void SkyBoxMaterial::LoadEffectVariables()
{
	m_pCubeMapSRVVariable = m_pEffect->GetVariableByName("m_CubeMap")->AsShaderResource();
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pCubeMapSRVVariable->SetResource(m_CubeMap->GetShaderResourceView());
}
