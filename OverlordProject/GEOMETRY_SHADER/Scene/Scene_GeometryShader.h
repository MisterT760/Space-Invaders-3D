#pragma once
#include "Scenegraph/GameScene.h"

class GameObject;
class Voxelizer_Explode;
class Voxelizer_Fall;
class Voxelizer_Disappear;
class SpriteFont;

class Scene_GeometryShader : public GameScene
{
public:
	Scene_GeometryShader();
	~Scene_GeometryShader();

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:

	float m_Timer, m_CubeSize = 1.5f, m_SlowMotion = 1.f, m_ExplosionForce = 500.f, m_Gravity = 50.f, m_OpacityFactor = 1.f;
	bool m_Activate, m_Rewind;
	GameObject* m_pModel = nullptr;
	Voxelizer_Explode* m_pExplodeMat = nullptr;
	Voxelizer_Fall* m_pFallMat = nullptr;
	Voxelizer_Disappear* m_pDisappearMat = nullptr;
	SpriteFont* m_pFont = nullptr;
	wstring m_EffectType;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Scene_GeometryShader(const Scene_GeometryShader &obj) = delete;
	Scene_GeometryShader& operator=(const Scene_GeometryShader& obj) = delete;
};

