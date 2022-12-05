#pragma once
#include "Button.h"
#include "CraftGreed.h"
class UiMgr;
class InvenItem;
class Player;
class Craft;
class CraftItem;
class CraftBox: public Button
{
private:
	const Vector2i startPos;
	int padding = 4;
	int width = 21;
	int height = 9;
	vector<vector<bool>> allPos;
	vector<vector<CraftGreed*>> itemGreed;
	vector<CraftItem*> items;
	CraftItem* nowDrag;
	Craft* inven;
	CraftBox* pairInven;
	Player* player;

public:
	bool isRequired = false;
	string craftingItmeName = "";
	CraftBox(UiMgr* mgr, Craft* inven, Vector2i startPos);
	virtual ~CraftBox();
	virtual void Init();
	virtual void Update(float dt);
	bool Crafting(string itemName);
	void DeletePlayerItem(string itemName);
	virtual void Draw(RenderWindow& window);
	void AddItem(string name, int count = 1);
	Vector2i FindInvenPos(int i_width, int i_height);
	CraftItem* GetNowDrag() { return nowDrag; }
	CraftGreed* GetGreed(int i, int j);
	void MoveItem(int i, int j);
	void ReturnItem();
	bool IsGreedItem(int i, int j) { return allPos[i][j]; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void SetDrag(CraftItem* nowDrag) { this->nowDrag = nowDrag; }
	void SetBoxSize(int width, int height);
	vector<CraftItem*>* GetItems() { return &items; }
	void ClearInven();
	void SetPair(CraftBox* pair) { this->pairInven = pair; }
	CraftBox* GetPair() { return pairInven; }
};

