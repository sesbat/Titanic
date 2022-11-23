#pragma once
#include "Button.h"
class UiMgr;
class InvenItem;
class InvenGreed;
class InventoryBox;

enum class InvenIdx
{
	None = -1, Weapon1, Weapon2, Cloth1, Cloth2, item1, item2, item3, item4, Count
};
class Inventory : public Button
{
private:
	float totalWeight;

	InvenItem* nowDrag;

	InventoryBox* myInven;
	InventoryBox* rightInven;
	InventoryBox* prevInven;

	Button* invenItemGreed[(int)InvenIdx::Count];
	vector<InvenItem*> myUseItems;

	int useIdx;


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
	InventoryBox* GetPrevInven();
	InventoryBox* GetRightInven();
	InventoryBox* GetPairBox(InventoryBox* now);
	void SetPrevInven(InventoryBox* inven) { this->prevInven = inven; }
	void MoveInvenItem(InventoryBox* nextInven);
	void ReturnUseItem();
	void ClearInven();

};