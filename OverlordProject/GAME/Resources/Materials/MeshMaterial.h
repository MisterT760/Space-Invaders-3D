#pragma once
#include "Graphics/Material.h"

class TextureData;

class MeshMaterial : public Material
{
public:
	MeshMaterial();
	~MeshMaterial();

	void SetDiffuseColor(XMFLOAT4 color) { m_DiffuseColor = color; }
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetInverted(bool inverted) { m_Inverted = inverted; }

	XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
	void SetLightDirection(XMFLOAT3 direction) { m_LightDirection = direction; };

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	XMFLOAT4 m_DiffuseColor = { 0,1,1,1 };
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;
	TextureData* m_DiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVVariable;
	bool m_Inverted = false;
	static ID3DX11EffectScalarVariable* m_pInverted;

	static ID3DX11EffectShaderResourceVariable* m_pShadowSRVvariable;

	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectMatrixVariable* m_pLightViewProjectionMatrix;
	XMFLOAT3 m_LightDirection;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshMaterial(const MeshMaterial &obj);
	MeshMaterial& operator=(const MeshMaterial& obj);
};

