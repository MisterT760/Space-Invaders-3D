#include "stdafx.h"

#include "DiffuseMaterial.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVVariable = nullptr;

DiffuseMaterial::DiffuseMaterial() : Material(L"./Resources/Effects/PosNormTex3D.fx"), m_DiffuseTexture(nullptr)
{
}


DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_DiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVVariable = Material::m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pDiffuseSRVVariable->SetResource(m_DiffuseTexture->GetShaderResourceView());
}
