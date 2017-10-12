#pragma once
#include "Scenegraph/GameScene.h"

class UberTestScene : public GameScene
{
public:
	UberTestScene(void);
	virtual ~UberTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pChair;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberTestScene(const UberTestScene &obj);
	UberTestScene& operator=(const UberTestScene& obj);
};

