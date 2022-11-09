#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;

class SceneDev2 :public Scene
{
	
protected:
	Player* player;
	Enemy* enemy;
	//SpriteObject* backGround;
	//
	float timer;

	//int boxCount;

	//map
	//VertexArrayObj* background;
	map<string, Vector2i> tileType;
	list<string> tileList; list<string> wallNum = { "58","41","60","2",
		"3","4","5","6","7","8","9","14","15","16",
		"17","18","19","20","21","26","27","28",
		"29","30","32","33","41","58","59","60" };
public:
	SceneDev2();
	virtual ~SceneDev2();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

};

