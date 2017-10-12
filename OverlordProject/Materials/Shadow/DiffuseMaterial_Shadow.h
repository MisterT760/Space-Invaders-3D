#pragma once
#include "Graphics/Material.h"

class TextureData;

class DiffuseMaterial_Shadow : public Material
{
public:
	DiffuseMaterial_Shadow();
	~DiffuseMaterial_Shadow();

	void SetDiffuseTexture(const wstring& assetFile);
	void SetLightDirection(XMFLOAT3 direction) { m_LightDirection = direction; };
	
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectShaderResourceVariable* m_pShadowSRVvariable;

	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectMatrixVariable* m_pLightViewProjectionMatrix;
	XMFLOAT3 m_LightDirection;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DiffuseMaterial_Shadow(const DiffuseMaterial_Shadow &obj);
	DiffuseMaterial_Shadow& operator=(const DiffuseMaterial_Shadow& obj);
};
