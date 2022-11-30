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

class GameScene :public Scene
{
public:
	struct Blocks
	{
		Vector2f position;
		candle::EdgeVector edgePool;
	};
protected:
	Player* player;

	list<Enemy*> enemies;


	float timer;
	Clock clock;

	IntRect mapSize;
	
	vector<SpriteObject*> sprObj;

	//mission
	Vector2f escapePoint;
	float escapeTimer;
	int killCount;
	float bombTimer;
	TextObject* missionText;
	vector<vector<bool>> isGreedObject;
	FloatRect MAP_BOUNDS = { 0, 0, 1920*4, 1080*4 };
	TreeRect treeRect = { 0,0,1920 * 4,1080 *4};
	QuadTree treeMap = QuadTree(treeRect, 16, 4);

	//view sight
	//std::vector<std::shared_ptr<candle::LightSource>> lights1;
	candle::RadialLight light;
	candle::LightingArea fog;
	
	//list<candle::EdgeVector>blockPool;
	vector<Blocks> blockPool;
	//Blocks blockPool[36*64];
	int blockCount;
public:
	GameScene();
	virtual ~GameScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	vector<HitBoxObject*> ObjListObb(HitBoxObject* obj);
	vector<HitBoxObject*> ObjListObb(FloatRect obj);
	virtual void Draw(RenderWindow& window) override;

	list<Enemy*>* GetEnemyList() { return &enemies; }
	Player* GetPlayer(){ return player; }
	float GetEscapeTimer() { return escapeTimer; }
	Vector2f GetEscapePoint() { return escapePoint; }
	void SetDeadEnemy(map<string, Item> items, Vector2f pos, Enemy* enemy);
	void DropItems(map<string, Item> items, Vector2f pos);
	void EmpytyInven(ItemBoxObject* inven);
	void SetDeadEnemy(map<string, Item> items, Vector2f pos);
	vector<vector<bool>> GetGreedObj() { return isGreedObject; }

	void pushEdge(const sfu::Line& edge);
	candle::EdgeVector pushBlock(const sf::Vector2f& pos);
	void castAllLights();
};