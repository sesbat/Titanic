#pragma once
#include "../Inventory.h"

class InventoryBox;
class Shop : public Inventory
{
	InventoryBox* playerInven;
	InventoryBox* npcInvne;
	InventoryBox* sellInven;
	InventoryBox* buyInven;

	vector<InvenItem*>* player_items;
	vector<InvenItem*>* npc_items;
	vector<InvenItem*>* sell_items;
	vector<InvenItem*>* buy_items;

	vector<InvenItem*> init_player_items;
	vector<InvenItem*> init_npc_items;

	map<InvenItem*, Vector2i> itemPrevPos;
public:
	Shop(UiMgr* mgr);
	~Shop();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetActive(bool state);
	virtual void SetDrag(InvenItem* item) {}
};

