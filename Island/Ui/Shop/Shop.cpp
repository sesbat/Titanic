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

	npcInvne = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
	npcInvne->Init();
	npcInvne->SetName("NpcInven");

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
	npc_items = npcInvne->GetItems();
	sell_items = sellInven->GetItems();
	buy_items = buyInven->GetItems();

	enabled = false;

	npcInvne->AddItem("Recoverykit",2);
	npcInvne->AddItem("Recoverykit", 2);
	npcInvne->AddItem("Apple",2);
	npcInvne->AddItem("Water",2);
	npcInvne->AddItem("EnergyDrink",2);
	npcInvne->AddItem("RifleBullet",100);

	price = 0;
	money = 0;

	money = ((Ready*)SCENE_MGR->GetCurrScene())->GetPlayer()->GetMoney();

	dealBtn = new Button(uimgr);
	dealBtn->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shop_deal.png"), true);
	dealBtn->SetClkColor(true);
	dealBtn->SetPos(Vector2f{ 960.f,175.f });
	dealBtn->SetOrigin(Origins::MC);

	txtPrice = new TextObject();
	txtPrice->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 20, Color::White, "DEAL " + to_string(price));
	txtPrice->SetPos(Vector2f{ 960.f,175.f });
	txtPrice->SetOrigin(Origins::MC);

	txtMoney = new TextObject();
	txtMoney->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 20, Color::White, to_string(money));
	txtMoney->SetPos(Vector2f{ 660.f,990.f });
	txtMoney->SetOrigin(Origins::MR);
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

				price += items->GetPrice() * items->GetCount();
				txtPrice->SetString("DEAL " + to_string(price));
				txtPrice->SetOrigin(Origins::MC);
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

				price -= items->GetPrice() * items->GetCount();
				txtPrice->SetString("DEAL " + to_string(price));
				txtPrice->SetOrigin(Origins::MC);
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

			price -= items->GetPrice() * items->GetCount();
			txtPrice->SetString("DEAL " + to_string(price));
			txtPrice->SetOrigin(Origins::MC);
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

			npcInvne->MoveItem(items, prev);

			itemPrevPos.erase(items);
			price += items->GetPrice() * items->GetCount();
			txtPrice->SetString("DEAL " + to_string(price));
			txtPrice->SetOrigin(Origins::MC);
			return;
		}
	}

	dealBtn->Update(dt);
	if (dealBtn->IsUp())
	{
		Deal();
	}
}

void Shop::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	playerInven->Draw(window);
	npcInvne->Draw(window);
	sellInven->Draw(window);
	buyInven->Draw(window);

	txtMoney->Draw(window);
	dealBtn->Draw(window);
	txtPrice->Draw(window);

}

void Shop::SetActive(bool state)
{
	bool prev = enabled;
	Inventory::SetActive(state);

	if (!prev && state)
	{

	}
	else if (prev && !state)
	{
		ClearShop();
	}
}

void Shop::ClearShop()
{
	for (auto items : *sell_items)
	{
		auto prev = itemPrevPos[items];
		playerInven->MoveItem(items, prev);
	}
	sell_items->clear();
	for (auto items : *buy_items)
	{
		auto prev = itemPrevPos[items];
		npcInvne->MoveItem(items, prev);
	}
	buy_items->clear();

	sellInven->Reset();
	buyInven->Reset();

	itemPrevPos.clear();
	price = 0.f;
}

void Shop::Deal()
{
	if (price < 0)
	{
		return;
	}

	for (auto items : *sell_items)
	{
		delete items;
	}
	sell_items->clear();
	for (auto items : *buy_items)
	{
		playerInven->AddItem(items->GetName(), items->GetCount());
		delete items;
	}
	buy_items->clear();

	sellInven->Reset();
	buyInven->Reset();

	itemPrevPos.clear();

	((Ready*)SCENE_MGR->GetCurrScene())->GetPlayer()->AddMoney(price);
	price = 0.f;
	money = ((Ready*)SCENE_MGR->GetCurrScene())->GetPlayer()->GetMoney();

	txtMoney->SetString(to_string(money));
	txtMoney->SetOrigin(Origins::MR);

	txtPrice->SetString("DEAL " + to_string(price));
	txtPrice->SetOrigin(Origins::MC);
}
