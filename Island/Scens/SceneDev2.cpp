#include "SceneDev2.h"
#include "SceneManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/SoundManager.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/VertexArrayObj.h"
#include "../../Framework/info.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../GameObject/MapObject.h"
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
	auto& data = FILE_MGR->GetMap("Tutorial");
	for (auto& obj : data)
	{
		if (obj.type == "TREE" || obj.type == "STONE")
		{
			MapObject* draw = new MapObject();
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			draw->SetOrigin(Origins::BC);

			int i = ((int)obj.position.x - 30) / 60;
			int j = (int)obj.position.y / 60 - 1;
			objList[LayerType::Object][0].push_back(draw);
		}
		else if(obj.type == "PLAYER")
		{
			player = new Player();
			player->SetName("Player");
			player->Init();
			player->SetPos(obj.position);
			//player->SetBackground(backGround);
			objList[LayerType::Object][0].push_back(player);
		}
		else if (obj.type == "ENEMY")
		{
			enemy = new Enemy();
			enemy->SetName("Enemy");
			enemy->Init(player);
			enemy->SetPos(obj.position);
			//enemy->SetBackground(backGround);
			objList[LayerType::Object][0].push_back(enemy);
		}
		else if (obj.type == "Tile")
		{
			SpriteObject* draw = new SpriteObject();
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);

			int i = ((int)obj.position.x - 30) / 60;
			int j = (int)obj.position.y / 60 - 1;
			objList[LayerType::Tile][0].push_back(draw);
		}
	}
}

void SceneDev2::Release()
{
	Scene::Release();
}

void SceneDev2::Enter()
{
	Init();
	//SCENE_MGR->GetCurrScene()->SetViewStop();
	//SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
}

void SceneDev2::Exit()
{
	Release();
}

void SceneDev2::Update(float dt)
{
	
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