#pragma once
#include "Graphics/Material.h"

class TextureData;

class Voxelizer_Disappear : public Material
{
public:
	Voxelizer_Disappear();
	~Voxelizer_Disappear();

	void SetDiffuseColor(const XMFLOAT4 &color) { m_DiffuseColor = color; }
	void SetCubeSize(const float &size) { m_CubeSize = size; }
	void SetTime(const float &time) { m_Time = time; }
	void SetOpacityFactor(const float &opacityFactor) { m_OpacityTimeFactor = opacityFactor; }

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	static ID3DX11EffectShaderResourceVariable* m_pNoiseTextureVariable;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseTextureVariable;
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;
	static ID3DX11EffectScalarVariable* m_pCubeSizeVariable;
	static ID3DX11EffectScalarVariable* m_pTimeVariable;
	static ID3DX11EffectScalarVariable* m_pOpacityTimeFactor;

	TextureData* m_pNoiseTexture;
	TextureData* m_pDiffuseTexture;
	XMFLOAT4 m_DiffuseColor;
	float m_CubeSize;
	float m_Time;
	float m_OpacityTimeFactor;
	

private:
		// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Voxelizer_Disappear(const Voxelizer_Disappear &obj) = delete;
	Voxelizer_Disappear& operator=(const Voxelizer_Disappear& obj) = delete;
};

