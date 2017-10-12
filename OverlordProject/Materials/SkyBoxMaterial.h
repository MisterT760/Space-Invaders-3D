#pragma once
#include "../../OverlordEngine/Graphics/Material.h"

class TextureData;
class ModelComponent;

class SkyBoxMaterial : public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();

	void SetCubeMapTexture(const std::wstring& assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	TextureData* m_CubeMap;
	static ID3DX11EffectShaderResourceVariable* m_pCubeMapSRVVariable;

private:
	SkyBoxMaterial(const SkyBoxMaterial &obj) = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj) = delete;

};

