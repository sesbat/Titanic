#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>
#include "../QTree/QuadTree.hpp"
#include "Candle/RadialLight.hpp"
#include "Candle/LightingArea.hpp"

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;
class TextObject;
class NPC;
class ItemBoxObject;

class GameScene :public Scene
{
	
protected:
	Player* player;

	list<Enemy*> enemies;

	QuadTree qTree;

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

	//view sight
	std::vector<std::shared_ptr<candle::LightSource>> lights1;
	candle::RadialLight light;
	candle::LightingArea fog;
	list<candle::EdgeVector>blockPool;
	candle::EdgeVector edgePool;
public:
	GameScene();
	virtual ~GameScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
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
	void pushBlock(const sf::Vector2f& pos);
	void castAllLights();
};