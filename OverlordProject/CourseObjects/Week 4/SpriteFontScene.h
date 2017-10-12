#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;

class SpriteFontScene : public GameScene
{
public:
	SpriteFontScene(void);
	virtual ~SpriteFontScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	SpriteFont* m_pFont = nullptr;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteFontScene(const SpriteFontScene &obj);
	SpriteFontScene& operator=(const SpriteFontScene& obj);
};

