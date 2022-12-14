#pragma once
#include "Button.h"
#include <map>
class UiMgr;
class InvenItem;
class CraftGreed;
class CraftBox;
class CraftItem;
class Player;

enum class CraftIdx
{
	None = -1,List,Required,Count,
};
class Craft : public Button
{
private:
	Craft(const Craft& ref) : Button(nullptr) {}
	Craft& operator=(const Craft& ref) {}
	Craft(Craft& ref) : Button(nullptr) {}
	Craft& operator=(Craft& ref) {}
	CraftItem* nowDrag;

	vector<Button*> categories;
	vector<CraftBox*> categoryBox;

	CraftItem* material;
	int categoryCount;

	CraftBox* craftBox;
	CraftBox* initRequiredItem;
	CraftBox* requiredItem;
	CraftBox* prevCraft;

	Button* craftGreed[(int)CraftIdx::Count];
	Player* player;

	bool isShow;

public:
	Craft(UiMgr* mgr);
	virtual ~Craft();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetDrag(CraftItem* item);
	CraftItem* GetNowDrag() { return nowDrag; }
	CraftGreed* GetGreed(int i, int j);
	CraftBox* GetNowInven();
	CraftBox* GetPrevInven();
	CraftBox* GetRightInven();
	CraftBox* GetPairBox(CraftBox* now);
	void SetPrevInven(CraftBox* inven) { this->prevCraft = inven; }
	void SetRightInven(CraftBox* inven) { requiredItem = inven; }
	void MoveInvenItem(CraftBox* nextInven);
	void ClearInven();
	void ResetRightInven();
	void OnClickCraftItem(const CraftingInfo& info);
	CraftBox* GetPlayerInven() { return craftBox; }

};