#pragma once
#include "UiMgr.h"
class Button;
class Inventory;
class SpriteObject;
class Player;
class NPC;
class ToolTip;
class GameSceneUiMgr : public UiMgr
{
protected:
	Player* player;

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

	Button* radiationBK;
	Button* radiation;
	Button* radiationTex;

	Button* ammoText;

	float hpBarSize;
	float staminaBarSize;
	ToolTip* tip;
public:
	ToolTip* GetTip() { return tip; }
	GameSceneUiMgr(Scene* scene);
	virtual ~GameSceneUiMgr();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Releas();
};

