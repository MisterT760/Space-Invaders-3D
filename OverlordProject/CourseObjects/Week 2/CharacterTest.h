#pragma once
#include "Scenegraph/GameScene.h"
#include "Character.h"

class CharacterTest : public GameScene
{
public:
	CharacterTest(void);
	virtual ~CharacterTest(void);


protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:
	Character* m_pCharacter;

private:
	CharacterTest(const CharacterTest &obj) = delete;
	CharacterTest& operator=(const CharacterTest& obj) = delete;
};

