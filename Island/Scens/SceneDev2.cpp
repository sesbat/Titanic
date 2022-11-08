#include "SceneDev2.h"
#include "SceneManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/SoundManager.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/VertexArrayObj.h"
#include "../../Framework/info.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;

SceneDev2::SceneDev2()
	:Scene(Scenes::Dev2), timer(0.f)
{
}

SceneDev2::~SceneDev2()
{
}

void SceneDev2::Init()
{
	backGround = new SpriteObject();
	backGround->SetTexture(*RESOURCES_MGR->GetTexture("graphics/Menu/back.png"));
	backGround->SetSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
	backGround->SetPos({ 0,0 });

	objList[LayerType::Back][0].push_back(backGround);

	player = new Player();
	player->SetName("Player");
	player->Init();
	//player->SetPos({ 1500,1300 });
	player->SetBackground(background);
	objList[LayerType::Object][0].push_back(player);

}

void SceneDev2::Release()
{
	Scene::Release();
}

void SceneDev2::Enter()
{
	Init();
	SCENE_MGR->GetCurrScene()->SetViewStop();
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	
}

void SceneDev2::Exit()
{
	Release();
}

void SceneDev2::Update(float dt)
{
	
	worldView.setCenter(player->GetPos());
	worldView.setCenter(player->GetPos());
	

	//dev modes
	
	//
	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
}