#pragma once
#include "Button.h"

class UiMgr;
class Inventory;
class InventoryBox;
class InvenGreed : public Button
{
private:
	Vector2i invenPos;
	Inventory* inven;
	InventoryBox* invenBox;
	bool state;
public:
	InvenGreed(UiMgr* mgr, Inventory* inven, InventoryBox* invenBox);
	~InvenGreed();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetState(bool s);
	void SetInvenPos(int i, int j) { invenPos = { i,j }; }
	void SetInvenPos(Vector2i iPos) { invenPos = iPos; }
	Vector2i GetInvenPos() { return invenPos; }

};

