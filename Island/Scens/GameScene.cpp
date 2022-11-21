#include "GameScene.h"
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
#include <cmath>
#include "../Ui/GameSceneUiMgr.h"


using namespace std;
using namespace sf;

GameScene::GameScene()
	:Scene(Scenes::GameScene), timer(0.f), escapeTimer(3.f)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	int id = 0;
	isMap = true;
	auto& data = FILE_MGR->GetMap("TUTORIAL");

	for (auto& obj : data)
	{
		if (obj.type == "TREE" || obj.type == "STONE" || obj.type == "BLOCK")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);

			objList[LayerType::Object][0].push_back(draw);
		}
		else if(obj.type == "PLAYER")
		{
			player = new Player();
			player->SetName(obj.type);
			player->Init();
			player->SetPos(obj.position);
			player->SetHitBox(obj.path);

			objList[LayerType::Object][0].push_back(player);
		}
		else if (obj.type == "ENEMY")
		{
			Enemy* enemy = new Enemy();
			enemy->SetName(obj.type);
			enemy->SetId(id++);
			enemy->SetPos(obj.position);
			enemy->SetHitBox(obj.path);
			enemies.push_back(enemy);

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

			objList[LayerType::Tile][0].push_back(draw);
		}
	}

	for (auto& enemy : enemies)
	{
		enemy->Init(player);
	}

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 30;
	mapSize.height = (tiles.back())->GetPos().y;

	
	//mission exit tile
	escapePoint = { 1200.f,1650.f };
	HitBoxObject* exit = new HitBoxObject();
	exit->SetName("EXIT");
	exit->SetTexture(*RESOURCES_MGR->GetTexture("graphics/exit.png"));
	exit->SetOrigin(Origins::BC);
	exit->SetPos(escapePoint);
	objList[LayerType::Tile][0].push_back(exit);

	missionText = new TextObject();
	missionText->SetActive(false);
	missionText->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 80, Color::Green, to_string(escapeTimer));
	missionText->SetTextLine(Color::Black, 1.f);
	missionText->SetOrigin(Origins::MC);
	missionText->SetPos(escapePoint);
	objList[LayerType::Object][1].push_back(missionText);
	//

	uiMgr = new GameSceneUiMgr(this);
	uiMgr->Init();
}

void GameScene::Release()
{
	Scene::Release();
	enemies.clear();
}

void GameScene::Enter()
{
	Init();

	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });

	SOUND_MGR->StopAll();
}

void GameScene::Exit()
{
	Release();
}

void GameScene::Update(float dt)
{
	//Time deltaTime = clock.restart();
	//float fps = 1.0f / (deltaTime.asSeconds());
	//cout << "fps: " << fps << endl;
	for (auto it = enemies.begin(); it != enemies.end(); )
	{
		if (!(*it)->GetActive())
		{
			it = enemies.erase(it);
		}
		else
			it++;
	}

	if (((GameSceneUiMgr*)uiMgr)->IsExit() || InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
		return;
	}

	LayerSort();
	

	Vector2f mouseworldPos = FRAMEWORK->GetWindow().mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), worldView);
	
	Vector2f dir;
	dir.x = mouseworldPos.x - player->GetPos().x;
	dir.y = mouseworldPos.y - player->GetPos().y;
	
	//camera
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

	//mission
	if (Utils::Distance(player->GetPos(), escapePoint) < 100.f)
	{
		escapeTimer -= dt;
		missionText->SetActive(true);
		missionText->SetString("LEAVING IN " + to_string(escapeTimer));
	}
	else
	{
		missionText->SetActive(false);
		escapeTimer = 3.f;
	}

	if (escapeTimer <= 0.f)
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
		return;
	}

	Scene::Update(dt);
}

void GameScene::Draw(RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
}