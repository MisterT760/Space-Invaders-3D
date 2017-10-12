#pragma once

class MeshFilter;
class ShadowMapMaterial;
class RenderTarget;
class ModelComponent;

class ShadowMapRenderer
{
public:
	ShadowMapRenderer();
	~ShadowMapRenderer();

	void Begin(const GameContext& gameContext);
	void Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, ModelComponent* pModelComponent, XMFLOAT4X4 world);
	void End(const GameContext& gameContext);
	XMFLOAT3 GetLightDirection() const { return m_LightDirection; }
	XMFLOAT4X4 GetLightVP() const { return m_LightVP; }
	ShadowMapMaterial* GetMaterial() const { return m_pShadowMapMat; }
	ID3D11ShaderResourceView* GetShadowSRV() const;
	void Initialize(const GameContext& gameContext);
	void SetLight(const XMFLOAT3& position, const XMFLOAT3& direction);

private:
	bool m_IsInitialized;
	XMFLOAT3 m_LightDirection, m_LightPosition;
	XMFLOAT4X4 m_LightVP;
	ShadowMapMaterial* m_pShadowMapMat;
	RenderTarget* m_pShadowRT;
	const float m_Size;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ShadowMapRenderer(const ShadowMapRenderer& t);
	ShadowMapRenderer& operator=(const ShadowMapRenderer& t);
};

