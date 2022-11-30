#pragma once
#include "Scene.h"
#include "../../GameObject/Animation/Animator.h"

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

	float timer;
	Clock clock;

	IntRect mapSize;
public:
	Ready();
	~Ready();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Reset() override;

	Player* GetPlayer() { return player; }
	NPC* GetStartNPC() { return startNpc; }
	NPC* GetShopNPC() { return shopNpc; }
	NPC* GetCraftNPC() { return craftNpc; }
};

