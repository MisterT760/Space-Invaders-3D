#pragma once
#include "Graphics/Material.h"

class TextureData;

class Voxelizer_Explode : public Material
{
public:
	Voxelizer_Explode();
	~Voxelizer_Explode();

	void SetDiffuseColor(const XMFLOAT4 &color) { m_DiffuseColor = color; }
	void SetCubeSize(const float &size) { m_CubeSize = size; }
	void SetExplosionForce(const float &ExplosionForce) { m_ExplosionForce = ExplosionForce; }
	void SetGravity(const float &gravity) { m_Gravity = gravity; }
	void SetTime(const float &time) { m_Time = time; }
	void SetSlowMotion(const float &slowMotion) { m_SlowMotionFactor = slowMotion; }
	void SetMaxTime(const float &max) { m_MaxExplosionTime = max; }

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	static ID3DX11EffectShaderResourceVariable* m_pNoiseTextureVariable;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseTextureVariable;
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;
	static ID3DX11EffectScalarVariable* m_pCubeSizeVariable;
	static ID3DX11EffectScalarVariable* m_pExplosionForceVariable;
	static ID3DX11EffectScalarVariable* m_pGravityVariable;
	static ID3DX11EffectScalarVariable* m_pTimeVariable;
	static ID3DX11EffectScalarVariable* m_pSlowMotionFactorVariable;
	static ID3DX11EffectScalarVariable* m_pMaxExplosionTimeVariable;

	TextureData* m_pNoiseTexture;
	TextureData* m_pDiffuseTexture;
	XMFLOAT4 m_DiffuseColor;
	float m_CubeSize;
	float m_ExplosionForce;
	float m_Gravity;
	float m_Time;
	float m_SlowMotionFactor;
	float m_MaxExplosionTime;
	

private:
		// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Voxelizer_Explode(const Voxelizer_Explode &obj);
	Voxelizer_Explode& operator=(const Voxelizer_Explode& obj);
};

