//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"

//#define W1
//#define W2
//#define W3
//#define W4
//#define W5
//#define W6
//#define W7
//#define W8
//#define W9
//#define W10

#define GAME
//#define GEOMETRY_SHADER

#ifdef W1
#include "CourseObjects/Week 1/TestScene.h"
#include "CourseObjects/Week 1/ComponentTestScene.h"
#include "CourseObjects/Week 1/PongScene.h"
#endif

#ifdef W2
#include "CourseObjects/Week 2/ModelTestScene.h"
#include "CourseObjects/Week 2/CharacterTest.h"
#endif

#ifdef W3
#include "CourseObjects/Week 3/SpriteTestScene.h"
#include "CourseObjects/Week 3/UberTestScene.h"
#endif

#ifdef W4
#include "CourseObjects/Week 4/SpriteFontScene.h"
#endif

#ifdef W5
#include "CourseObjects/Week 5/SkyBoxTestScene.h"
#include "CourseObjects/Week 5/HeightmapTestScene.h"
#endif

#ifdef W6
#include "CourseObjects/Week 6/SoftwareSkinningScene_1.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_2.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_3.h"
#endif

#ifdef W7
#include "CourseObjects/Week 7/HardwareSkinningScene.h"
#endif

#ifdef W8
#include "CourseObjects/Week 8/PostProcessingScene.h"
#endif

#ifdef W9
#include "CourseObjects/Week 9/ShadowMappingScene.h"
#endif

#ifdef W10
#include "CourseObjects/Week 10/ParticleTestScene.h"
#endif

#ifdef GAME
#include "GAME/Scenes/Scene_MainMenu.h"
#include "GAME/Scenes/Scene_HelpMenu.h"
#include "GAME/Scenes/Scene_MainLevel.h"
#endif

#ifdef GEOMETRY_SHADER
#include "GEOMETRY_SHADER/Scene/Scene_GeometryShader.h"
#endif

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{

#ifdef W1
	SceneManager::GetInstance()->AddGameScene(new TestScene());
	SceneManager::GetInstance()->AddGameScene(new ComponentTestScene());
	SceneManager::GetInstance()->AddGameScene(new PongScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"TestScene");
#endif

#ifdef W2
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTest());

	//SceneManager::GetInstance()->SetActiveGameScene(L"ModelTestScene");
#endif

#ifdef W3
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	SceneManager::GetInstance()->AddGameScene(new UberTestScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"SpriteTestScene");
#endif

#ifdef W4
	SceneManager::GetInstance()->AddGameScene(new SpriteFontScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"SpriteFontScene");
#endif

#ifdef W5
	SceneManager::GetInstance()->AddGameScene(new SkyBoxTestScene());
	SceneManager::GetInstance()->AddGameScene(new HeightmapTestScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"HeightmapTestScene");
#endif

#ifdef W6
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());

	//SceneManager::GetInstance()->SetActiveGameScene(L"SoftwareSkinningScene_3");
#endif

#ifdef W7
	SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"HardwareSkinningScene");
#endif

#ifdef W8
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"PostProcessingScene");
#endif

#ifdef W9
	SceneManager::GetInstance()->AddGameScene(new ShadowMappingScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"ShadowMappingScene");
#endif

#ifdef W10
	SceneManager::GetInstance()->AddGameScene(new ParticleTestScene());

	//SceneManager::GetInstance()->SetActiveGameScene(L"ParticleTestScene");
#endif

#ifdef GAME
	SceneManager::GetInstance()->AddGameScene(new Scene_MainMenu());
	SceneManager::GetInstance()->AddGameScene(new Scene_HelpMenu());
	SceneManager::GetInstance()->AddGameScene(new Scene_MainLevel());

	SceneManager::GetInstance()->SetActiveGameScene(L"Scene_MainMenu");
#endif

#ifdef GEOMETRY_SHADER
	SceneManager::GetInstance()->AddGameScene(new Scene_GeometryShader());

	SceneManager::GetInstance()->SetActiveGameScene(L"Scene_GeometryShader");
#endif
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				//SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				//SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				//DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
