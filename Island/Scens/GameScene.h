#pragma once
#include "Scene.h"
#include "../GameObject/Object.h"
#include <map>
#include <list>

class Player;
class Enemy;
class VertexArrayObj;
class SpriteObject;

class GameScene :public Scene
{
	
protected:
	Player* player;
	
	//
	float timer;

	Vector2f prevWorldPos;
	IntRect mapSize;

public:
	GameScene();
	virtual ~GameScene();

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	Player* GetPlayer(){ return player; }
};