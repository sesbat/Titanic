#include "SceneManager.h"
#include "Menu.h"
#include "Ready.h"
#include "GameScene.h"
#include "MapEditor.h"
#include "../Framework/info.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Release()
{
	for (auto& scene : sceneMap)
	{
		delete scene.second;
	}
}

Scene* SceneManager::GetCurrScene()
{
	return sceneMap[currScene];
}
Scenes SceneManager::GetCurrSceneType()
{
	return currScene;
}

Scene* SceneManager::GetScene(Scenes scene)
{
	return sceneMap[scene];
}

bool SceneManager::LoadPath()
{
	rapidcsv::Document doc(RESOURCES_MGR->FilePath, rapidcsv::LabelParams(0, -1));

	int col = doc.GetColumnCount();
	for (int i = 0; i < col; i++)
	{
		paths.push_back((doc.GetColumn<string>(1)[i]) + RESOURCES_CSV);
	}
	for (auto& path : paths)
	{
		PRINT(path);
	}
	return true;

}

bool SceneManager::Init()
{
	LoadPath();

	sceneMap[Scenes::Menu] = new Menu();
	sceneMap[Scenes::Ready] = new Ready();
	sceneMap[Scenes::GameScene] = new GameScene();
	sceneMap[Scenes::MapEditor] = new MapEditor();

	currScene = Scenes::Menu;
	sceneMap[currScene]->SetSceneName("READYSCENE");
	sceneMap[currScene]->Enter();
	return false;
}

void SceneManager::ChangeScene(Scenes scene)
{
	InputMgr::Clear();
	sceneMap[currScene]->Exit();
	currScene = scene;
	sceneMap[currScene]->Enter();
}

void SceneManager::ChangeScene(Scenes scene, string sceneName)
{
	InputMgr::Clear();
	sceneMap[currScene]->Exit();
	currScene = scene;
	sceneMap[currScene]->SetSceneName(sceneName);
	sceneMap[currScene]->Enter();
}

void SceneManager::Update(float dt)
{
	sceneMap[currScene]->Update(dt);
}

void SceneManager::Draw(RenderWindow& window)
{
	sceneMap[currScene]->Draw(window);
}
