#pragma once
#include "Scene.h"
#include "../../GameObject/Animation/Animator.h"
#include "../3rd/QuadTree_SFML_DEMO.h"

class SpriteObject;
class Button;
class NPC;
class Player;
class Inventory;
class Ready : public Scene
{
protected:
	Player* player;

	NPC* startNpc;
	NPC* shopNpc;
	NPC* craftNpc;
	NPC* healNpc;

	float timer;
	Clock clock;

	IntRect mapSize;
	FloatRect MAP_BOUNDS = { 0, 0, 1920 * 4, 1080 * 4 };
	TreeRect treeRect = { 0,0,1920 * 4,1080 * 4 };
	QuadTree treeMap;

	SpriteObject* cursor;
public:
	Ready();
	~Ready();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float dt) override;
	vector<HitBoxObject*> ObjListObb(HitBoxObject* obj);
	vector<HitBoxObject*> ObjListObb(FloatRect obj);
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	Player* GetPlayer() { return player; }
	NPC* GetStartNPC() { return startNpc; }
	NPC* GetShopNPC() { return shopNpc; }
	NPC* GetCraftNPC() { return craftNpc; }
	NPC* GetHealNPC() { return healNpc; }

};

