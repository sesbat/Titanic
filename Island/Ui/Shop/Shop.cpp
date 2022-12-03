#include "Shop.h"
#include "../../Framework/ResourceManager.h"
#include "../../Framework/InputMgr.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include "../../Framework/FileManager.h"
#include "../../Framework/info.h"
#include "../../GameObject/Player.h"
#include "../../Scens/SceneManager.h"
#include "../../Scens/Ready.h"
#include "../InvenGreed.h"
#include "../InvenItem.h"
#include "../InventoryBox.h"

Shop::Shop(UiMgr* mgr)
	:Inventory(mgr)
{

}

Shop::~Shop()
{
}

void Shop::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/shop.png"), true);

	rightInven = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
	rightInven->Init();
	rightInven->SetName("NpcInven");

	sellInven = new InventoryBox(uimgr, this, Vector2i{ 768,256 });
	sellInven->SetInvenSize(6, 5);
	sellInven->Init();
	sellInven->SetName("SellInven");

	buyInven = new InventoryBox(uimgr, this, Vector2i{ 768,640 });
	buyInven->SetInvenSize(6, 5);
	buyInven->Init();
	buyInven->SetName("BuyInven");

	playerInven = ((Ready*)SCENE_MGR->GetCurrScene())->GetPlayer()->GetInventory()->GetPlayerInven();
	playerInven->Init();

	player_items = playerInven->GetItems();
	npc_items = rightInven->GetItems();
	sell_items = sellInven->GetItems();
	buy_items = buyInven->GetItems();


}

void Shop::Update(float dt)
{
	if (!enabled)
		return;

	for (auto items : *player_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			auto move_pos = sellInven->FindInvenPos(items->GetWidth(), items->GetHeight());
			if (move_pos != Vector2i{ -1,-1 })
			{
				itemPrevPos[items] = items->GetGreedPos();
				playerInven->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });
				sellInven->MoveItem(items, Vector2i{move_pos.y, move_pos.x});
			}

			return;
		}
	}
	for (auto items : *npc_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			auto move_pos = buyInven->FindInvenPos(items->GetWidth(), items->GetHeight());
			if (move_pos != Vector2i{ -1,-1 })
			{
				itemPrevPos[items] = items->GetGreedPos();
				npcInvne->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });
				buyInven->MoveItem(items, Vector2i{ move_pos.y, move_pos.x });
			}
			return;
		}
	}

	for (auto items : *sell_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			sellInven->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });

			auto prev = itemPrevPos[items];

			playerInven->MoveItem(items, prev);
			itemPrevPos.erase(items);
			return;
		}
	}

	for (auto items : *buy_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			buyInven->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });

			auto prev = itemPrevPos[items];

			playerInven->MoveItem(items, prev);

			itemPrevPos.erase(items);
			return;
		}
	}
}

void Shop::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	playerInven->Draw(window);
	rightInven->Draw(window);
	sellInven->Draw(window);
	buyInven->Draw(window);

}

void Shop::SetActive(bool state)
{
	bool prev = enabled;
	Inventory::SetActive(state);

	if (!prev && state)
	{
		init_player_items = *player_items;
		init_npc_items = *npc_items;
	}
}

