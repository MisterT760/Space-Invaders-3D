#include "stdafx.h"
#include "Voxelizer_Disappear.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"


ID3DX11EffectShaderResourceVariable* Voxelizer_Disappear::m_pDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* Voxelizer_Disappear::m_pNoiseTextureVariable = nullptr;
ID3DX11EffectVectorVariable* Voxelizer_Disappear::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Disappear::m_pCubeSizeVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Disappear::m_pTimeVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Disappear::m_pOpacityTimeFactor = nullptr;

Voxelizer_Disappear::Voxelizer_Disappear() : Material(L"./GEOMETRY_SHADER/Effect/Voxelizer_Disappear.fx"),
                                             m_pNoiseTexture(nullptr), m_pDiffuseTexture(nullptr),
                                             m_CubeSize(0), m_Time(0), m_OpacityTimeFactor(0)
{
}


Voxelizer_Disappear::~Voxelizer_Disappear()
{
}

void Voxelizer_Disappear::LoadEffectVariables()
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(L"./GEOMETRY_SHADER/Resources/Textures/Texture_Diffuse.png");
	m_pNoiseTexture = ContentManager::Load<TextureData>(L"./GEOMETRY_SHADER/Resources/Textures/Texture_Noise.png");

	if (!m_pDiffuseTextureVariable)
	{
		m_pDiffuseTextureVariable = m_pEffect->GetVariableByName("gDiffuseTexture")->AsShaderResource();
		if (!m_pDiffuseTextureVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gDiffuseTexture\' variable not found!");
			m_pDiffuseTextureVariable = nullptr;
		}
	}

	if (!m_pNoiseTextureVariable)
	{
		m_pNoiseTextureVariable = m_pEffect->GetVariableByName("gNoiseTexture")->AsShaderResource();
		if (!m_pNoiseTextureVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gNoiseTexture\' variable not found!");
			m_pNoiseTextureVariable = nullptr;
		}
	}

	if (!m_pDiffuseColorVariable)
	{
		m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gColor")->AsVector();
		if (!m_pDiffuseColorVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gDiffuseColor\' variable not found!");
			m_pDiffuseColorVariable = nullptr;
		}
	}

	if (!m_pCubeSizeVariable)
	{
		m_pCubeSizeVariable = m_pEffect->GetVariableByName("gCubeSize")->AsScalar();
		if (!m_pCubeSizeVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gCubeSize\' variable not found!");
			m_pCubeSizeVariable = nullptr;
		}
	}

	if (!m_pTimeVariable)
	{
		m_pTimeVariable = m_pEffect->GetVariableByName("gTimer")->AsScalar();
		if (!m_pTimeVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gTimer\' variable not found!");
			m_pTimeVariable = nullptr;
		}
	}

	if (!m_pOpacityTimeFactor)
	{
		m_pOpacityTimeFactor = m_pEffect->GetVariableByName("gOpacityTimeFactor")->AsScalar();
		if (!m_pOpacityTimeFactor->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gOpacityTimeFactor\' variable not found!");
			m_pOpacityTimeFactor = nullptr;
		}
	}
}

void Voxelizer_Disappear::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTextureVariable)
	{
		m_pDiffuseTextureVariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	if (m_pNoiseTextureVariable)
	{
		m_pNoiseTextureVariable->SetResource(m_pNoiseTexture->GetShaderResourceView());
	}

	if (m_pDiffuseColorVariable)
	{
		m_pDiffuseColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_DiffuseColor));
	}

	if (m_pCubeSizeVariable)
	{
		m_pCubeSizeVariable->SetFloat(m_CubeSize);
	}

	if (m_pTimeVariable)
	{
		m_pTimeVariable->SetFloat(m_Time);
	}

	if (m_pOpacityTimeFactor)
	{
		m_pOpacityTimeFactor->SetFloat(m_OpacityTimeFactor);
	}
}
