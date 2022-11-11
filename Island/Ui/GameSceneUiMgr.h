#pragma once
#include "UiMgr.h"
class Button;
class GameSceneUiMgr : public UiMgr
{
protected:
	Button* exitBtn;
public:
	GameSceneUiMgr(Scene* scene);
	virtual ~GameSceneUiMgr();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	bool IsExit();
};

