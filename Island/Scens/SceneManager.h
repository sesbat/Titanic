#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <map>
#include "../3rd/SingleTon.h"

using namespace std;
using namespace sf;
class SceneManager : public Singleton<SceneManager>
{
private:
	vector<string> paths;
	map<Scenes, Scene*> sceneMap;
	Scenes currScene;
public:
	~SceneManager();
	void Release();
	Scene* GetCurrScene();
	Scene* GetScene(Scenes scene);


	bool LoadPath();
	bool Init();
	void ChangeScene(Scenes scene);
	void Update(float dt);
	void Draw(RenderWindow& window);
};

#define SCENE_MGR (SceneManager::GetInstance())