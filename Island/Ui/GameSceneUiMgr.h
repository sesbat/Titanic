#pragma once
#include "UiMgr.h"
class Button;
class Inventory;
class SpriteObject;
class Player;
class GameSceneUiMgr : public UiMgr
{
protected:
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

	Player* player;

	float hungerGuage;
	int prevHungerGuage;
	float thirstGuage;
	int prevThirstGuage;
	float energyGuage;
	int prevEnergyGuage;

	float staminaTime;
	float dash;
	float hpBarSize;
	float time;
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

