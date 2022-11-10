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
	
	//
	float timer;

	Vector2f prevWorldPos;

public:
	SceneDev2();
	virtual ~SceneDev2();

	// Sceneï¿½ï¿½(ï¿½ï¿½) ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Óµï¿?
	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

