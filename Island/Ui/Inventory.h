#pragma once
#include "Button.h"
class UiMgr;
class InvenItem;
class InvenGreed;
class InventoryBox;
class Inventory : public Button
{
private:
	float totalWeight;

	InvenItem* nowDrag;

	InventoryBox* myInven;
	InventoryBox* rightInven;
	InventoryBox* prevInven;

public:
	Inventory(UiMgr* mgr);
	virtual ~Inventory();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetDrag(InvenItem* item);
	InvenItem* GetNowDrag() { return nowDrag; }
	InvenGreed* GetGreed(int i, int j);
	InventoryBox* GetNowInven();
	void SetPrevInven(InventoryBox* inven) { this->prevInven = inven; }
	void MoveInvenItem(InventoryBox* nextInven);
};