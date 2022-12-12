#pragma once
#include "../UiMgr.h"
#include <vector>


class ReadyButton;
class Scene;
class Player;
class NPC;
class Button;
class Inventory;
class Craft;
class TextObject;
class Shop;
class BoolWindowBox;
class ReadyUiMgr : public UiMgr
{
private:
	Player* player;
	NPC* startNpc;
	NPC* craftNpc;
	NPC* shopNpc;
	NPC* healNpc;

	BoolWindowBox* yesORno;
	Button* healed; //you have been healed
	float messageTime;

	Craft* craft;
	Shop* shop;

	UiMgr* uiMgr;

	vector<Button*> maps;
	Button* mapsBK;

	vector<Button*> categories;
	Button* categoryBK;

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

	float hpBarSize;
	float staminaBarSize;
public:
	ReadyUiMgr(Scene* scene);
	~ReadyUiMgr();
	virtual void Init();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

