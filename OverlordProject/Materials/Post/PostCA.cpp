//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PostCA.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"

PostCA::PostCA()
	: PostProcessingMaterial(L"./Resources/Effects/Post/PostCA.fx"),
	  m_pTextureMapVariabele(nullptr), m_pTextureNoiseVariable(nullptr)
{
}

PostCA::~PostCA()
{
}


void PostCA::LoadEffectVariables()
{
	m_pNoiseImage = ContentManager::Load<TextureData>(L"./GAME/Resources/Textures/Noise_Texture.png");

	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
	{
		Logger::LogWarning(L"PostCA-> gTexture is invalid");
	}

	m_pTextureNoiseVariable = m_pEffect->GetVariableByName("gNoiseTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
	{
		Logger::LogWarning(L"PostCA-> gNoiseTexture is invalid");
	}

	m_pDistortionOffsetsVariable = m_pEffect->GetVariableByName("gDistortionOffsets")->AsVector();
	if (!m_pDistortionOffsetsVariable->IsValid())
	{
		Logger::LogWarning(L"PostCA-> gDistortionOffsets is invalid");
	}

	m_pDistortionTimeVariable = m_pEffect->GetVariableByName("gDistortionTime")->AsScalar();
	if (!m_pDistortionTimeVariable->IsValid())
	{
		Logger::LogWarning(L"PostCA-> gDistortionTime is invalid");
	}

	m_pDistortionFrequencyVariable = m_pEffect->GetVariableByName("gDistortionFrequency")->AsScalar();
	if (!m_pDistortionFrequencyVariable->IsValid())
	{
		Logger::LogWarning(L"PostCA-> gDistortionFrequency is invalid");
	}

}

void PostCA::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	m_pTextureNoiseVariable->SetResource(m_pNoiseImage->GetShaderResourceView());
	m_pDistortionOffsetsVariable->SetFloatVector(reinterpret_cast<float*>(&m_DistortionOffsets));
	m_pDistortionTimeVariable->SetFloat(m_DistortionTime);
	m_pDistortionFrequencyVariable->SetFloat(m_DistortionFrequency);
}
