#pragma once
#include "Button.h"

class UiMgr;
class Inventory;
class InventoryBox;
class InvenItem;
class InvenGreed : public Button
{
private:
	InvenGreed(const InvenGreed& ref) : Button(nullptr) {}
	InvenGreed& operator=(const InvenGreed& ref) {}
	InvenGreed(InvenGreed& ref) : Button(nullptr) {}
	InvenGreed& operator=(InvenGreed& ref) {}
	Vector2i invenPos;
	Inventory* inven;
	InventoryBox* invenBox;
	bool state;
	InvenItem* nowItem;
public:
	InvenGreed(UiMgr* mgr, Inventory* inven, InventoryBox* invenBox);
	virtual ~InvenGreed();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetState(bool s);
	virtual void SetState(bool s, InvenItem* itme);
	void SetInvenPos(int i, int j) { invenPos = { i,j }; }
	void SetInvenPos(Vector2i iPos) { invenPos = iPos; }
	Vector2i GetInvenPos() { return invenPos; }
	InvenItem* GetItem() { return nowItem; }

};

