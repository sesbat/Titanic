#pragma once
#include "Button.h"
#include "InvenGreed.h"
class UiMgr;
class InvenItem;
class Inventory : public Button
{
private:
	const Vector2i startPos1 = Vector2i{ 163,252 };
	const Vector2i startPos2 = Vector2i{ 1248,252 };
	const int padding = 4;

	const int width = 8;
	const int height = 11;

	float totalWeight;

	vector<vector<bool>> itemPos;
	vector<vector<InvenGreed*>> itemGreed;

	vector<InvenItem*> items;
	InvenItem* nowDrag;

public:
	Inventory(UiMgr* mgr);
	virtual ~Inventory();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void AddItem(string name);
	Vector2i FindInvenPos(int i_width, int i_height);
	InvenItem* GetNowDrag() { return nowDrag; }
	InvenGreed* GetGreed(int i, int j);
	void MoveItem(int i, int j);
	void ReturnItem();
	bool IsGreedItem(int i, int j) { return itemPos[i][j]; }
};