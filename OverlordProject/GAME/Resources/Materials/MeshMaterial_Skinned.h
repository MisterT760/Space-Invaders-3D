#pragma once
#include "Graphics/Material.h"

class TextureData;

class MeshMaterial_Skinned : public Material
{
public:
	MeshMaterial_Skinned();
	~MeshMaterial_Skinned();

	void SetDiffuseColor(XMFLOAT4 color) { m_DiffuseColor = color; }
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetInverted(bool inverted) { m_Inverted = inverted; }
	void SetLightDirection(const XMFLOAT3& direction) { m_LightDirection = direction; };

	XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	XMFLOAT4 m_DiffuseColor = { 0,1,1,1 };
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;
	TextureData* m_DiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVVariable;
	bool m_Inverted = false;
	static ID3DX11EffectScalarVariable* m_pInverted;
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;

	static ID3DX11EffectShaderResourceVariable* m_pShadowSRVvariable;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectMatrixVariable* m_pLightViewProjectionMatrix;
	XMFLOAT3 m_LightDirection;


private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshMaterial_Skinned(const MeshMaterial_Skinned &obj);
	MeshMaterial_Skinned& operator=(const MeshMaterial_Skinned& obj);
};

