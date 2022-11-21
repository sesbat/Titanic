#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;
class TextObject;

class GameScene :public Scene
{
	
protected:
	Player* player;
	list<Enemy*> enemies;

	float timer;
	Clock clock;

	IntRect mapSize;

	

	//mission
	Vector2f escapePoint;
	float escapeTimer;
	int killCount;
	float bombTimer;
	TextObject* missionText;

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
};