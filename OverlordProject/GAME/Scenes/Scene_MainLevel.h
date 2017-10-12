#pragma once
#include "Scenegraph/GameScene.h"
#include "Graphics/SpriteFont.h"

class PlayerPrefab;
class FixedCamera;
class LaserPrefab;
class BlockPrefab;
class SmallInvader;
class EnemyLaserPrefab;
class PickupPrefab;
class SpriteFont;
class BaseButtonPrefab;

class Scene_MainLevel :public GameScene
{
public:
	Scene_MainLevel(void);
	virtual ~Scene_MainLevel(void);

protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	template<class T> void RemoveFromVec(T* ptr, vector<T*>& vec);

private:
	PlayerPrefab* m_pPlayer = nullptr;
	GameObject* m_pLaserKill = nullptr, *m_pLeftStop = nullptr, *m_pRightStop = nullptr, *m_pFrontStop = nullptr;
	
	//Terrain
	float m_TerrainSpeed = 50;
	vector<GameObject*> m_pTerrain;

	//Lasers
	vector<LaserPrefab*> m_pLasers;
	vector<EnemyLaserPrefab*> m_pEnemyLasers;

	//Enemies
	vector<SmallInvader*> m_pEnemies;
	int m_EnemyLevel = 0;

	//PickUps
	vector<PickupPrefab*> m_pPickups;

	//Blocks
	BlockPrefab* m_pBlockLeft = nullptr, *m_pBlockRight = nullptr;

	//HUD
	GameObject* m_pHUD_Health_Back = nullptr, *m_pHUD_Health_Front = nullptr, *m_pHUD_Heart = nullptr,
				*m_pHUD_Score_Back = nullptr, *m_pHUD_Score_Front = nullptr, *m_pHUD_Shield = nullptr,
				*m_pHUD_Shield_Back = nullptr, *m_pHUD_Shield_Front = nullptr;

	SpriteFont* m_pHUD_Font32 = nullptr, * m_pHUD_Font64 = nullptr;
	int m_Score = 0, m_Buttons = 0;
	bool m_End = false;
	BaseButtonPrefab* m_pExit = nullptr;

	//Managing
	void ManageLasers(const GameContext& gameContext);
	void ManageEnemies(const GameContext& gameContext);
	void ManageBlocks(const GameContext& gameContext);
	void ManageTerrain(const GameContext& gameContext);
	void ManageHUD(const GameContext& gameContext) const;
	void ManagePickUps(const GameContext& gameContext);

	//Spawning
	void SpawnEnemies(const GameContext& gameContext);
	void SpawnPickUps(const GameContext& gameContext);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Scene_MainLevel(const Scene_MainLevel &obj) = delete;
	Scene_MainLevel& operator=(const Scene_MainLevel& obj) = delete;
};

template <class T>
void Scene_MainLevel::RemoveFromVec(T* ptr, vector<T*>& vec)
{
	if (ptr)
	{
		for (typename vector<T*>::iterator it = vec.begin(); it != vec.end();)
		{
			if (ptr == *it)
			{
				RemoveChild(*it);
				it = vec.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

