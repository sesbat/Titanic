#pragma once
#include "Button.h"
#include "InvenGreed.h"
class UiMgr;
class InvenItem;
class InventoryBox : public Button
{
private:
	const Vector2i startPos;
	const int padding = 4;
	const int width = 8;
	const int height = 11;
	vector<vector<bool>> itemPos;
	vector<vector<InvenGreed*>> itemGreed;
	vector<InvenItem*> items;
	InvenItem* nowDrag;
	Inventory* inven;
public:
	InventoryBox(UiMgr* mgr, Inventory* inven, Vector2i startPos);
	virtual ~InventoryBox();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void AddItem(string name, int count = 1);
	Vector2i FindInvenPos(int i_width, int i_height);
	InvenItem* GetNowDrag() { return nowDrag; }
	InvenGreed* GetGreed(int i, int j);
	void MoveItem(int i, int j);
	void ReturnItem();
	bool IsGreedItem(int i, int j) { return itemPos[i][j]; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void SetDrag(InvenItem* nowDrag) { this->nowDrag = nowDrag; }
	vector<InvenItem*>* GetItems() { return &items; }
};

