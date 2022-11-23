#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>
#include "../QTree/QuadTree.hpp"

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;
class TextObject;
class NPC;

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
	void SetDeadEnemy(map<string, Item> items, Vector2f pos);
};