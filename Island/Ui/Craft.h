#pragma once
#include "Button.h"
class UiMgr;
class InvenItem;
class CraftGreed;
class CraftBox;

enum class CraftIdx
{
	None = -1,List,Required,Count,
};
class Craft : public Button
{
private:
	float totalWeight;

	InvenItem* nowDrag;

	vector<Button*> categories;
	int categoryCount;

	CraftBox* craftBox;
	CraftBox* initRequiredItem;
	CraftBox* requiredItem;
	CraftBox* prevCraft;

	Button* craftGreed[(int)CraftIdx::Count];
	vector<InvenItem*> myUseItems;

	int useIdx;


public:
	Craft(UiMgr* mgr);
	virtual ~Craft();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetDrag(InvenItem* item);
	InvenItem* GetNowDrag() { return nowDrag; }
	CraftGreed* GetGreed(int i, int j);
	CraftBox* GetNowInven();
	CraftBox* GetPrevInven();
	CraftBox* GetRightInven();
	CraftBox* GetPairBox(CraftBox* now);
	void SetPrevInven(CraftBox* inven) { this->prevCraft = inven; }
	void SetRightInven(CraftBox* inven) { requiredItem = inven; }
	void MoveInvenItem(CraftBox* nextInven);
	void ReturnUseItem();
	void ClearInven();
	void ResetRightInven();
	CraftBox* GetPlayerInven() { return craftBox; }

};