#pragma once
#include "stdafx.h"
#include "../../OverlordEngine/Graphics\Material.h"

class ShadowMapMaterial :
	public Material
{
public:
	ShadowMapMaterial(const wstring& effectFile, const wstring& technique = L"");
	~ShadowMapMaterial();
	void SetLightVP(XMFLOAT4X4 lightVP);
	void SetWorld(XMFLOAT4X4 world);
	void EnableBones(bool enable);

protected:
	friend class ShadowMapRenderer;

	void LoadEffectVariables();
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);
private:
	static ID3DX11EffectScalarVariable* m_pHasBonesVariable;
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;
	static ID3DX11EffectMatrixVariable* m_pLightVPMatrixVariable;
	static ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable;

	XMFLOAT4X4 m_LightVP;
	XMFLOAT4X4 m_World;
	bool m_HasBones;

	ShadowMapMaterial(const ShadowMapMaterial &obj);
	ShadowMapMaterial& operator=(const ShadowMapMaterial& obj);
};

