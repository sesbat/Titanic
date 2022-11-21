#pragma once
#include "UiMgr.h"
class Button;
class Inventory;
class SpriteObject;
class Player;
class NPC;
class GameSceneUiMgr : public UiMgr
{
protected:
	Player* player;

	Button* exitBtn;
	Inventory* inven;

	Button* hpBar;
	Button* hpBarBK;
	Button* hpSprite;

	Button* staminaBar;
	Button* staminaBK;
	Button* staminaSprite;

	Button* hunger;
	Button* hungerBK;
	Button* hungerTex;

	Button* thirst;
	Button* thirstBK;
	Button* thirstTex;

	Button* energy;
	Button* energyBK;
	Button* energyTex;

	float hpBarSize;
	float staminaBarSize;
public:
	GameSceneUiMgr(Scene* scene);
	virtual ~GameSceneUiMgr();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void SetPlayer(Player* player);
	bool IsExit();
};

