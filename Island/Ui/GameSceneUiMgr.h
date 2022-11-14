#pragma once
#include "UiMgr.h"
class Button;
class Inventory;
class GameSceneUiMgr : public UiMgr
{
protected:
	Button* exitBtn;
	Inventory* inven;
public:
	GameSceneUiMgr(Scene* scene);
	virtual ~GameSceneUiMgr();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	bool IsExit();
};

