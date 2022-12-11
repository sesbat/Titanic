#pragma once
#include "HitBoxObject.h"

class InvenGreed;
class InvenItem;
class InventoryBox;
class Ment;
class Player;
class ItemBoxObject : public HitBoxObject
{
protected:
	ItemBoxObject(const ItemBoxObject& ref) {}
	ItemBoxObject& operator=(const ItemBoxObject& ref) {}
	ItemBoxObject( ItemBoxObject& ref) {}
	ItemBoxObject& operator=( ItemBoxObject& ref) {}
	bool pickState;
	map<string, Item> obj_items;
	Vector2f* playerPos;

	InventoryBox* invenBox;

	Player* player;
	Ment* ment;

public:
	ItemBoxObject();
	~ItemBoxObject();
	virtual void Init();
	void SetPlayer(Player* player);
	virtual void Reset();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetItems(map<string, Item> items);
	void SetPlayerPos(Vector2f* pos) { playerPos = pos; }
	InventoryBox* GetInvenBox() { return invenBox; }
};

