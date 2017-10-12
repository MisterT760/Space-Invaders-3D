#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;
class TextureData;

class PostCA : public PostProcessingMaterial
{
public:
	PostCA();
	~PostCA();

	void SetDistortionTime(const float time) { m_DistortionTime = time; }
	void SetDistortionOffsets(const XMFLOAT3 offsets) { m_DistortionOffsets = offsets; }

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* rendertarget) override;

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
	ID3DX11EffectShaderResourceVariable* m_pTextureNoiseVariable;
	ID3DX11EffectVectorVariable* m_pDistortionOffsetsVariable;
	ID3DX11EffectScalarVariable* m_pDistortionTimeVariable;
	ID3DX11EffectScalarVariable* m_pDistortionFrequencyVariable;

	TextureData* m_pNoiseImage = nullptr;
	XMFLOAT3 m_DistortionOffsets = { 0.f,0.f,0.f };
	float m_DistortionTime = 0.5f, m_DistortionFrequency = 0.25f;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostCA(const PostCA &obj);
	PostCA& operator=(const PostCA& obj);
};
