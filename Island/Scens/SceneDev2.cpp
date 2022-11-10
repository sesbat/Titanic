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
#include "../GameObject/HitBoxObject.h"
#include <fstream>
#include <algorithm>

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
	isMap = true;
	auto& data = FILE_MGR->GetMap("Tutorial");
	for (auto& obj : data)
	{
		if (obj.type == "TREE" || obj.type == "STONE")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);

			draw->SetHitBox(obj.path);

			int i = ((int)obj.position.x - 30) / 60;
			int j = (int)obj.position.y / 60 - 1;
			objList[LayerType::Object][0].push_back(draw);
		}
		else if(obj.type == "PLAYER")
		{
			player = new Player();
			player->SetName(obj.type);
			player->Init();
			player->SetPos(obj.position);

			player->SetHitBox(obj.path);

			//player->SetBackground(backGround);
			objList[LayerType::Object][0].push_back(player);
		}
		else if (obj.type == "ENEMY")
		{
			enemy = new Enemy();
			enemy->SetName(obj.type);
			enemy->Init(player);
			enemy->SetPos(obj.position);

			enemy->SetHitBox(obj.path);

			//enemy->SetBackground(backGround);
			objList[LayerType::Object][0].push_back(enemy);
		}
		else if (obj.type == "TILE")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);

			draw->SetHitBox(obj.path);

			int i = ((int)obj.position.x - 30) / 60;
			int j = (int)obj.position.y / 60 - 1;
			objList[LayerType::Tile][0].push_back(draw);
		}
	}
	prevWorldPos = player->GetPos();

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 30;
	mapSize.height = (tiles.back())->GetPos().y;
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
	//worldView.setCenter(player->GetPos());
	LayerSort();
	//worldView.setCenter(player->GetPos());
	//dev modes
	Vector2f mouseworldPos = FRAMEWORK->GetWindow().mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), worldView);
	
	Vector2f dir;
	dir.x = mouseworldPos.x - player->GetPos().x;
	dir.y = mouseworldPos.y - player->GetPos().y;

	float r = 0.1;
	Vector2f camPoslen;
	camPoslen.x = dir.x * r;
	camPoslen.y = dir.y * r;


	Vector2f realcam;
	realcam.x = camPoslen.x + player->GetPos().x;
	realcam.y = camPoslen.y + player->GetPos().y;

	realcam.x = max((int)realcam.x, WINDOW_WIDTH / 2);
	realcam.x = min((int)realcam.x, mapSize.width -  WINDOW_WIDTH / 2);
	realcam.y = max((int)realcam.y, WINDOW_HEIGHT / 2);
	realcam.y = min((int)realcam.y, mapSize.height - WINDOW_HEIGHT / 2);

	worldView.setCenter(realcam);
	
	

	//if (player->GetActive())
	//{
	//	if (worldView.getCenter().x - worldView.getSize().x * 0.5f < mapSize.left)
	//	{
	//		worldView.setCenter(mapSize.left + worldView.getSize().x * 0.5f, player->GetPos().y - 200);
	//	}
	//	else if (worldView.getCenter().x + worldView.getSize().x * 0.5f > mapSize.left + mapSize.width)
	//	{
	//		worldView.setCenter(mapSize.left + mapSize.width - worldView.getSize().x * 0.5f, player->GetPos().y - 200);
	//	}
	//	else
	//	{
	//		worldView.setCenter(realcam);
	//		//prevWorldPos = realcam;
	//	}

	//	if (worldView.getCenter().y - worldView.getSize().y * 0.5f < mapSize.top)
	//	{
	//		worldView.setCenter(mapSize.top + worldView.getSize().y * 0.5f, player->GetPos().y - 200);
	//	}
	//	else if (worldView.getCenter().y + worldView.getSize().y * 0.5f > mapSize.top + mapSize.height)
	//	{
	//		worldView.setCenter(mapSize.top + mapSize.height - worldView.getSize().y * 0.5f, player->GetPos().y - 200);
	//	}
	//	else
	//	{
	//		worldView.setCenter(realcam);
	//		//prevWorldPos = realcam;
	//	}
		
	//}
	
	Scene::Update(dt);
}

void SceneDev2::Draw(RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
}