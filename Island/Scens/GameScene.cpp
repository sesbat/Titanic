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
#include "../GameObject/NPC.h"
#include "../GameObject/ItemBoxObject.h"
#include "../Ui/Inventory.h"
#include "../Ui/InventoryBox.h"
#include "Candle/geometry/Polygon.hpp"

#include "../GameObject/HitBox.h"
	
using namespace std;
using namespace sf;

//explicit QuadTree(sf::FloatRect bounds,	size_t maxLevel, size_t maxObjects);
GameScene::GameScene()
	:Scene(Scenes::GameScene), timer(0.f), escapeTimer(3.f),
	fog(candle::LightingArea::FOG,
		sf::Vector2f(0.f, 0.f),
		sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT * 4)), blockCount(0)
{

}

GameScene::~GameScene()
{
	min(0, 1);
}

void GameScene::Init()
{
	int id = 0;
	isMap = true;
	auto& data = FILE_MGR->GetMap(sceneName);
	isGreedObject.clear();

	for (int i = 0; i < 72; i++)
		isGreedObject.push_back(vector<bool>(128, false));

	player = new Player();
	player->SetName("PLAYER");
	player->Init();

	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
	}

	//blockPool = new Blocks[36*64];

	for (auto& obj : data)
	{
		if (obj.type == "STONE"   || obj.type == "BLOCK")
		{
			isGreedObject[obj.greedIdx.x][obj.greedIdx.y] = true;
			Blocks block = { obj.position, pushBlock(obj.position) };
			blockPool.push_back(block);
			//blockCount++;
		}
		if (obj.type == "BOX" || obj.type == "BOX-ENEMY")
		{
			ItemBoxObject* box = new ItemBoxObject();
			box->SetPlayerPos(player->GetPosPtr());
			box->SetItems(obj.item);
			box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/box.png"));
			box->SetName(obj.type);
			box->SetOrigin(Origins::BC);
			box->SetPos(obj.position);
			box->SetHitBox(obj.path);
			objList[LayerType::Object][0].push_back(box);
		}
		else if (obj.type == "TREE" || obj.type == "BUSH" || 
			obj.type == "STONE" || obj.type == "BLOCK")
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
			enemy->SetItem(obj.item);
			enemy->SetGreedObject(&isGreedObject);
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
		else if (obj.type == "ANOTHER")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);
			escapePoint = obj.position;
			escapePoint.y -= 100.f;

			objList[LayerType::Object][0].push_back(draw);

			missionText = new TextObject();
			missionText->SetActive(false);
			missionText->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 80, Color::Green, to_string(escapeTimer));
			missionText->SetTextLine(Color::Black, 1.f);
			missionText->SetOrigin(Origins::BC);
			missionText->SetPos(escapePoint);
			objList[LayerType::Object][1].push_back(missionText);
			//HitBoxObject* exit = new HitBoxObject();
			//exit->SetTexture(*RESOURCES_MGR->GetTexture("graphics/exit.png"));
			//exit->SetOrigin(Origins::BC);
			//exit->SetPos(obj.position);
			//objList[LayerType::Object][0].push_back(exit);
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

	{
		treeMap.setFont(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"));
	}
	
	//mission exit tile
	//escapePoint = { 1200.f,1650.f };

	//missionText = new TextObject();
	//missionText->SetActive(false);
	//missionText->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 80, Color::Green, to_string(escapeTimer));
	//missionText->SetTextLine(Color::Black, 1.f);
	//missionText->SetOrigin(Origins::MC);
	//missionText->SetPos(escapePoint);
	//objList[LayerType::Object][1].push_back(missionText);
	
	//view sight
	light.setRange(800.f);

	//fog.setMode(candle::LightingArea::FOG);
	//fog.setScale({ WINDOW_WIDTH,WINDOW_HEIGHT });
	fog.setAreaColor(Color(0, 0, 0, 200));

	uiMgr = new GameSceneUiMgr(this);
	uiMgr->Init();
}

void GameScene::Release()
{
	Scene::Release();
	enemies.clear();
	blockPool.clear();
}

void GameScene::Enter()
{
	enemies.clear();

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
	//FloatRect v;
	//v.width = uiView.getSize().x;
	//v.height = uiView.getSize().y;
	//v.left = uiView.getCenter().x - 1920 / 2;
	//v.top = uiView.getCenter().y - 1080 / 2;
	//qTree.SetBound(v);
	
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

	if(!player->GetInventory()->GetActive())
		worldView.setCenter(realcam);

	//view sight pos
	light.setPosition(player->GetPos());
	//castAllLights();	

	//mission
	if (Utils::Distance(player->GetPos(), escapePoint) < 100.f)
	{
		//string et = to_string(escapeTimer);
		//et = et.substr(0, et.find('.') + 2);
		escapeTimer -= dt;
		missionText->SetActive(true);
		string timer = to_string(escapeTimer); 
		timer = timer.substr(0, timer.find('.') + 3);
		missionText->SetString("LEAVING IN " + timer);
	}
	else
	{
		missionText->SetActive(false);
		escapeTimer = 3.f;
	}

	if (escapeTimer <= 0.f)
	{
		SCENE_MGR->ChangeScene(Scenes::Ready);
		return;
	}
	if (!player->GetIsAlive())
	{
		SCENE_MGR->ChangeScene(Scenes::Ready);
		return;
	}

	treeMap.clear();
	LayerSort();

	//treeMap.insert(objList[LayerType::Object][0]);
	treeMap.insert(drawObjs);
	treeMap.update(drawObjs);

	//for (auto&& found : treeMap.getObjectsInBound_unchecked(*player)) {
	//	if (player != found && Utils::OBB(player->GetBottom()->GetHitbox(), found->GetBottom()->GetHitbox())) {
	//		found->SetColor(Color::Red);
	//		cout << found->GetName() << endl;
	//	}
	//}
	Scene::Update(dt);
}
vector<HitBoxObject*> GameScene::ObjListObb(HitBoxObject* obj)
{
	return treeMap.getObjectsInBound_unchecked_notParent(*obj);
}
vector<HitBoxObject*> GameScene::ObjListObb(FloatRect obj)
{
	return treeMap.getObjectsInBound_unchecked_notParent(obj);
}

	
void GameScene::Draw(RenderWindow& window)
{
	window.setView(worldView);
	LayerSort();
	int i = 0;
	for (auto& obj : objList[LayerType::Tile])
	{
		for (auto& o : obj.second)
		{
			o->Draw(window);
		}
	}
	for (auto& obj : another)
	{
		obj->Draw(window);
	}
	for (auto& obj : drawObjs)
	{
		obj->Draw(window);
	}
	for (auto& obj : objList[LayerType::Object])
	{
		if (obj.first == 0)
			continue;
		for (auto& o : obj.second)
		{
			o->Draw(window);
		}
	}

	fog.clear();
	fog.draw(light);
	//window.draw(light);
	window.draw(fog);
	fog.display();

	if (uiMgr != nullptr)
		uiMgr->Draw(window);

	//window.setView(worldView);

	//{
	//	window.setView(worldView);


	//	int i = 0;
	//	for (auto& obj : objList[LayerType::Tile])
	//	{
	//		for (auto& o : obj.second)
	//		{
	//			o->Draw(window);
	//		}
	//	}

	//	for (auto& obj : another)
	//	{
	//		obj->Draw(window);
	//	}
	//	for (auto& obj : drawObjs)
	//	{
	//		obj->Draw(window);
	//	}
	//	for (auto& obj : objList[LayerType::Object])
	//	{
	//		if (obj.first == 0)
	//			continue;
	//		for (auto& o : obj.second)
	//		{
	//			o->Draw(window);
	//		}
	//	}

	//	if (uiMgr != nullptr)
	//		uiMgr->Draw(window);
	//	
	//}
	//{
	//	window.setView(worldView);
	//	treeMap.draw(window);
	//}
	//Scene::Draw(window);
}

void GameScene::SetDeadEnemy(map<string, Item> items, Vector2f pos, Enemy* enemy)
{
	ItemBoxObject* box = new ItemBoxObject();
	box->SetItems(items);
	box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/enemy1-die.png"));

	box->SetOrigin(Origins::MC);
	box->SetHitBox("graphics/enemy1-die.png");
	box->SetPos(pos);
	box->SetName("BOX-ENEMY");
	box->SetPlayerPos(player->GetPosPtr());

	auto boxPos = ((HitBoxObject*)(box))->GetBottomPos() + box->GetGlobalBound().height / 2;

	auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), enemy);
	objList[LayerType::Object][0].insert(it, box);
}

void GameScene::DropItems(map<string, Item> items, Vector2f pos)
{
	ItemBoxObject* box = new ItemBoxObject();
	box->SetItems(items);
	box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/box.png"));

	box->SetOrigin(Origins::MC);
	box->SetHitBox("graphics/items/box.png");
	box->SetPos(pos);
	box->SetName("BOX");
	box->SetPlayerPos(player->GetPosPtr());

	auto boxPos = ((HitBoxObject*)(box))->GetBottomPos() + box->GetGlobalBound().height / 2;

	auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), player);
	objList[LayerType::Object][0].insert(it, box);
}

void GameScene::EmpytyInven(ItemBoxObject* inven)
{
	if (inven == nullptr)
		return;

	if (inven->GetName() == "BOX")
	{
		auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), inven);
		deleteContainer.push_back(it);
		(*it)->SetActive(false);
	}
}

void GameScene::pushEdge(const sfu::Line& edge)
{
	//edgePool.push_back(edge);
}

candle::EdgeVector GameScene::pushBlock(const sf::Vector2f& pos)
{
	candle::EdgeVector edge;
	const sf::Vector2f points[] = {
			{pos.x - 30, pos.y -75.f} ,
			{pos.x + 30, pos.y -75.f} ,
			{pos.x + 30, pos.y } ,
			{pos.x - 30, pos.y } ,
	};
	sfu::Polygon p(points, 4);
	for (auto& l : p.lines)
	{
		edge.push_back(l);
	}
	return edge;
}

void GameScene::castAllLights()
{
	for (auto &it : blockPool)
	{
		light.castLight(it.edgePool.begin(), it.edgePool.end());
		/*if ((Utils::Distance(player->GetPos(), it.position) >= light.getRange()/2))
		{
			continue;
		}
		else
		{
			light.castLight(it.edgePool.begin(), it.edgePool.end());
		}*/
	}
	/*for (int i = 0; i < blockCount; i++)
	{
		if ((Utils::Distance(player->GetPos(), blockPool[i].position) >= light.getRange()))
		{
			continue;
		}
		else
		{
			light.castLight(blockPool[i].edgePool.begin(), blockPool[i].edgePool.end());
		}
		
	}*/
}
