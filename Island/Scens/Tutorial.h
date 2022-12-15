#pragma once
#include "Scene.h"

class SpriteObject;
class Button;
class Tutorial : public Scene
{
private:
	vector<SpriteObject*> png;
	int idx;

public:
	Tutorial();
	virtual ~Tutorial();

	virtual void Enter();
	virtual void Exit();
	virtual void Init();
	virtual void Reset();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

