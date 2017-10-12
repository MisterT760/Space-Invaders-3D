#include "stdafx.h"
#include "Voxelizer_Explode.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"


ID3DX11EffectShaderResourceVariable* Voxelizer_Explode::m_pDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* Voxelizer_Explode::m_pNoiseTextureVariable = nullptr;
ID3DX11EffectVectorVariable* Voxelizer_Explode::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pCubeSizeVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pExplosionForceVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pGravityVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pTimeVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pSlowMotionFactorVariable = nullptr;
ID3DX11EffectScalarVariable* Voxelizer_Explode::m_pMaxExplosionTimeVariable = nullptr;

Voxelizer_Explode::Voxelizer_Explode() : Material(L"./GEOMETRY_SHADER/Effect/Voxelizer_Explode.fx"),
                                         m_pNoiseTexture(nullptr), m_pDiffuseTexture(nullptr),
                                         m_CubeSize(0), m_ExplosionForce(0), m_Gravity(0), m_Time(0),
                                         m_SlowMotionFactor(0), m_MaxExplosionTime(0)
{
}


Voxelizer_Explode::~Voxelizer_Explode()
{
}

void Voxelizer_Explode::LoadEffectVariables()
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

	if (!m_pExplosionForceVariable)
	{
		m_pExplosionForceVariable = m_pEffect->GetVariableByName("gExplosionForce")->AsScalar();
		if (!m_pExplosionForceVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gExplosionForce\' variable not found!");
			m_pExplosionForceVariable = nullptr;
		}
	}

	if (!m_pGravityVariable)
	{
		m_pGravityVariable = m_pEffect->GetVariableByName("gGravity")->AsScalar();
		if (!m_pGravityVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gGravity\' variable not found!");
			m_pGravityVariable = nullptr;
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

	if (!m_pSlowMotionFactorVariable)
	{
		m_pSlowMotionFactorVariable = m_pEffect->GetVariableByName("gSlowMotionFactor")->AsScalar();
		if (!m_pSlowMotionFactorVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gSlowMotionFactor\' variable not found!");
			m_pSlowMotionFactorVariable = nullptr;
		}
	}

	if (!m_pMaxExplosionTimeVariable)
	{
		m_pMaxExplosionTimeVariable = m_pEffect->GetVariableByName("gMaxExplosionTime")->AsScalar();
		if (!m_pMaxExplosionTimeVariable->IsValid())
		{
			Logger::LogWarning(L"MeshMaterialSkinned::LoadEffectVariables() > \'gSlowMotionFactor\' variable not found!");
			m_pMaxExplosionTimeVariable = nullptr;
		}
	}
}

void Voxelizer_Explode::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
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

	if (m_pExplosionForceVariable)
	{
		m_pExplosionForceVariable->SetFloat(m_ExplosionForce);
	}

	if (m_pGravityVariable)
	{
		m_pGravityVariable->SetFloat(m_Gravity);
	}

	if (m_pTimeVariable)
	{
		m_pTimeVariable->SetFloat(m_Time);
	}

	if (m_pSlowMotionFactorVariable)
	{
		m_pSlowMotionFactorVariable->SetFloat(m_SlowMotionFactor);
	}

	if (m_pMaxExplosionTimeVariable)
	{
		m_pMaxExplosionTimeVariable->SetFloat(m_MaxExplosionTime);
	}
}
