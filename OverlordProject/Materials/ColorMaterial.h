#pragma once
#include "Graphics\Material.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();

	void SetColor(const XMFLOAT4 &color) { m_Color = color; }

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	static ID3DX11EffectVectorVariable* m_pColorVariable;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);
};

