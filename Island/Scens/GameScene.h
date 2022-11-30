#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>\

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;
class TextObject;
class NPC;
class ItemBoxObject;
#include "../3rd/QuadTree_SFML_DEMO.h"

class GameScene :public Scene
{
	
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
	FloatRect MAP_BOUNDS = { 0, 0, 1920*2, 1080*2 };
	TreeRect treeRect = { 0,0,1920 * 2,1080 *2};
	QuadTree treeMap = QuadTree(treeRect, 16, 4);

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
};