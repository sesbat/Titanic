#pragma once
#include "../Inventory.h"

class InventoryBox;
class TextObject;
class Shop : public Inventory
{
	InventoryBox* playerInven;
	//InventoryBox* npcInvne;
	InventoryBox* sellInven;
	InventoryBox* buyInven;

	vector<Button*> shopCategorybtn;
	vector<InventoryBox*> shopCategoryBox;

	vector<InvenItem*>* player_items;
	vector<InvenItem*>* npc_items;
	vector<InvenItem*>* sell_items;
	vector<InvenItem*>* buy_items;


	map<InvenItem*, Vector2i> itemPrevPos;
	map<InvenItem*, InventoryBox*> prevItemBox;

	Button* dealBtn;
	TextObject* txtPrice;
	
	vector<map<string, ShopItemInfo>> info;
	int price;
	int money;
public:
	Shop(UiMgr* mgr);
	~Shop();
	virtual void Release();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetActive(bool state);
	void ClearShop();
	void Deal();
	virtual void SetDrag(InvenItem* item) {}
};

