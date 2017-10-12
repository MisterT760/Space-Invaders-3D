#include "stdafx.h"

#include "UberMaterial.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"

ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable = nullptr;

UberMaterial::UberMaterial() : Material(L"./Resources/Effects/UberShader.fx"),
m_bDiffuseTexture(false),
m_pDiffuseTexture(nullptr),
m_bSpecularLevelTexture(false),
m_pOpacityMap(nullptr),
m_bSpecularBlinn(false),
m_bSpecularPhong(false),
m_bFresnelFaloff(false),
m_FresnelPower(1),
m_FresnelMultiplier(1),
m_FresnelHardness(1),
m_pNormalMappingTexture(nullptr),
m_bEnvironmentMapping(false),
m_pEnvironmentCube(nullptr),
m_ReflectionStrength(1),
m_RefractionStrength(1),
m_RefractionIndex(1),
m_Opacity(100),
m_bOpacityMap(false),
m_pSpecularLevelTexture(nullptr),
m_Shininess(20),
m_AmbientIntensity(0),
m_bFlipGreenChannel(false),
m_bNormalMapping(false)
{
}

UberMaterial::~UberMaterial()
{
}

void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

void UberMaterial::FlipNormalGreenChannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

void UberMaterial::EnableFresnelFalloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	//Diffuse
	if (m_bDiffuseTexture)
	{
		m_pDiffuseSRVvariable = Material::m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	}
	
	m_pUseDiffuseTextureVariable = Material::m_pEffect->GetVariableByName("gUseTextureDiffuse")->AsScalar();
	m_pDiffuseColorVariable = Material::m_pEffect->GetVariableByName("gColorDiffuse")->AsVector();

	//Specular
	m_pSpecularLevelSRVvariable = Material::m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();

	if (m_bSpecularLevelTexture)
	{
		m_pSpecularLevelSRVvariable = Material::m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
	}

	m_pSpecularColorVariable = Material::m_pEffect->GetVariableByName("gColorSpecular")->AsVector();
	m_pUseSpecularLevelTextureVariable = Material::m_pEffect->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
	m_pShininessVariable = Material::m_pEffect->GetVariableByName("gShininess")->AsScalar();

	m_pUsePhongVariable = Material::m_pEffect->GetVariableByName("gUseSpecularPhong")->AsScalar();
	m_pUseBlinnVariable = Material::m_pEffect->GetVariableByName("gUseSpecularBlinn")->AsScalar();

	//Ambient
	m_pAmbientColorVariable = Material::m_pEffect->GetVariableByName("gColorAmbient")->AsVector();
	m_pAmbientIntensityVariable = Material::m_pEffect->GetVariableByName("gAmbientIntensity")->AsScalar();

	//Normal

	if (m_bNormalMapping)
	{
		m_pNormalMappingSRVvariable = Material::m_pEffect->GetVariableByName("gTextureNormal")->AsShaderResource();
	}

	m_pFlipGreenChannelVariable = Material::m_pEffect->GetVariableByName("gFlipGreenChannel")->AsScalar();
	m_pUseNormalMappingVariable = Material::m_pEffect->GetVariableByName("gUseTextureNormal")->AsScalar();
	
	//Environment

	if (m_bEnvironmentMapping)
	{
		m_pEnvironmentSRVvariable = Material::m_pEffect->GetVariableByName("gCubeEnvironment")->AsShaderResource();
	}

	m_pUseEnvironmentMappingVariable = Material::m_pEffect->GetVariableByName("gUseTextureEnvironment")->AsScalar();
	m_pReflectionStrengthVariable = Material::m_pEffect->GetVariableByName("gReflectionStrength")->AsScalar();
	m_pRefractionStrengthVariable = Material::m_pEffect->GetVariableByName("gRefractionStrength")->AsScalar();
	m_pRefractionIndexVariable = Material::m_pEffect->GetVariableByName("gRefractionIndex")->AsScalar();

	//Fresnel
	m_pUseFresnelFalloffVariable = Material::m_pEffect->GetVariableByName("gUseFresnelFalloff")->AsScalar();
	m_pFresnelColorVariable = Material::m_pEffect->GetVariableByName("gColorFresnel")->AsVector();
	m_pFresnelPowerVariable = Material::m_pEffect->GetVariableByName("gFresnelPower")->AsScalar();
	m_pFresnelMultiplierVariable = Material::m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
	m_pFresnelHardnessVariable = Material::m_pEffect->GetVariableByName("gFresnelHardness")->AsScalar();

	//Opacity

	if (m_bOpacityMap)
	{
		m_pOpacitySRVvariable = Material::m_pEffect->GetVariableByName("gTextureOpacity")->AsShaderResource();
	}

	m_pOpacityVariable = Material::m_pEffect->GetVariableByName("gOpacityIntensity")->AsScalar();
	m_pUseOpacityMapVariable = Material::m_pEffect->GetVariableByName("gUseTextureOpacity")->AsScalar();
}

void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	//Diffuse
	if (m_bDiffuseTexture)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
	m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	m_pDiffuseColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorDiffuse));

	//Specular
	if (m_bSpecularLevelTexture)
	{
		m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	}
	m_pSpecularColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorSpecular));
	m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	m_pShininessVariable->SetInt(m_Shininess);

	m_pUsePhongVariable->SetBool(m_bSpecularPhong);
	m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);

	//Ambient
	m_pAmbientColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorAmbient));
	m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);

	//Normal
	if (m_bNormalMapping)
	{
		m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}
	m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);

	//Environment
	if (m_bEnvironmentMapping)
	{
		m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	}
	m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);
	

	//Fresnel
	m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	m_pFresnelColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorFresnel));
	m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);

	//Opacity
	if (m_bOpacityMap)
	{
		m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	}
	m_pOpacityVariable->SetFloat(m_Opacity);
	m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);

}
