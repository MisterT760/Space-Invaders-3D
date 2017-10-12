//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ColorMaterial.h"
#include "Base/GeneralStructs.h"

ID3DX11EffectVectorVariable* ColorMaterial::m_pColorVariable = nullptr;

ColorMaterial::ColorMaterial(bool enableTransparency) : 
	Material(L"./Resources/Effects/PosNormCol3D.fx", enableTransparency?L"TransparencyTech":L"")
{
}


ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::LoadEffectVariables()
{
	m_pColorVariable = m_pEffect->GetVariableByName("gColor")->AsVector();
}

void ColorMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_Color));
}
