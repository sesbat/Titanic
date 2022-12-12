#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>
#include "Candle/RadialLight.hpp"
#include "Candle/LightingArea.hpp"
#include "../3rd/QuadTree_SFML_DEMO.h"

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;
class TextObject;
class NPC;
class ItemBoxObject;
class Boss;

class GameScene :public Scene
{
public:
	struct Blocks
	{
		Vector2f position;
		candle::EdgeVector edgePool;
	};
protected:
	GameScene& operator=(const GameScene& ref) {}
	GameScene& operator=(GameScene& ref) {}

	Player* player;
	list<Enemy*> enemies;
	list<Boss*> bosses;
	Boss* boss;

	float timer;
	Clock clock;
	IntRect mapSize;
	
	//mission
	Vector2f escapePoint;
	float escapeTimer;
	int killCount;
	float bombTimer;
	TextObject* missionText;
	vector<vector<bool>> isGreedObject;
	FloatRect MAP_BOUNDS = { 0, 0, 1920*4, 1080*4 };
	TreeRect treeRect = { 0,0,1920 * 4,1080 *4};
	QuadTree treeMap;

	//view sight
	candle::RadialLight light;
	candle::LightingArea fog;
	vector<Blocks> blockPool;
	int blockCount;

	SpriteObject* cursor;
	SpriteObject* shot_cursor;

	bool targeting = false;
	VertexArray lines;

	bool isZoom;
	float r = 0.1f;
	float MaxR = 0.4f;
	float MinR = 0.1f;
	float zoomInOutSpeed = 0.01f;

	float supplyTimer;
	float initSupplyTimer;
	bool isSupply;
public:
	GameScene();
	virtual ~GameScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	void SupplyUpdate(float dt);
	vector<HitBoxObject*> ObjListObb(HitBoxObject* obj);
	vector<HitBoxObject*> ObjListObb(FloatRect obj);
	virtual void Draw(RenderWindow& window) override;

	list<Enemy*>* GetEnemyList() { return &enemies; }
	list<Boss*>* GetBossList() { return &bosses; }
	Player* GetPlayer(){ return player; }
	float GetEscapeTimer() { return escapeTimer; }
	Vector2f GetEscapePoint() { return escapePoint; }
	void SetDeadEnemy(map<string, Item> items, Vector2f pos, Enemy* enemy);
	void DropItems(map<string, Item> items, Vector2f pos);
	void EmpytyInven(ItemBoxObject* inven);
	//void SetDeadEnemy(map<string, Item> items, Vector2f pos);
	vector<vector<bool>> GetGreedObj() { return isGreedObject; }

	candle::EdgeVector pushBlock(const sf::Vector2f& pos);
	void castAllLights();
};