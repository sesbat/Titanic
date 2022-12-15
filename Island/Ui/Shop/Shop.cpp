#include "Shop.h"
#include "../../Framework/ResourceManager.h"
#include "../../Framework/InputMgr.h"
#include "../../Framework/Framework.h"
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
	Release();
}

void Shop::Release()
{
	for(auto& box : shopCategoryBox)
	{
		if (box != nullptr)
			delete box;
		box = nullptr;
	}

	/*if (npcInvne != nullptr)
		delete npcInvne;
	npcInvne = nullptr;*/

	if (sellInven != nullptr)
		delete sellInven;
	sellInven = nullptr;

	if (buyInven != nullptr)
		delete buyInven;
	buyInven = nullptr;

	if (txtMoney != nullptr)
		delete txtMoney;
	txtMoney = nullptr;

	if (dealBtn != nullptr)
		delete dealBtn;
	dealBtn = nullptr;

	if (txtPrice != nullptr)
		delete txtPrice;
	txtPrice = nullptr;
}

void Shop::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/shop.png"), true);

	String itemTab[] = { "trader_consumable_icon", "trader_armor_icon", "trader_ammo_icon", "trader_weapon_icon"};

	for (int i = 0; i < 4; i++)
	{
		Button* button = new Button(uimgr);
		button->SetClkColor(true);
		button->SetTexture(*RESOURCES_MGR->GetTexture("graphics/" + itemTab[i] + ".png"),true);
		//button->SetOrigin(Origins::MC);
		button->SetPos({ FRAMEWORK->GetWindowSize().x - 130.f,252 + i * 64.f });
		shopCategorybtn.push_back(button);
	}

	String box[] = { "shopBox1", "shopBox2", "shopBox3","shopBox4"};
	for (int i = 0; i < 4; i++)
	{
		auto shopBox = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
		shopBox->SetClkColor(true);
		shopBox->Init();
		shopBox->SetName(box[i]);
		shopBox->SetActive(false);
		shopCategoryBox.push_back(shopBox);
	}

	info = FILE_MGR->GetAllShopItem();

	int i = 0;
	for (auto& shopPage : info)
	{
		for (auto& shopItem : shopPage)
		{
			shopCategoryBox[i]->AddItem(shopItem.first,shopItem.second.cnt);
		}
		i++;
	}

	shopCategoryBox[0]->SetActive(true);

	/*npcInvne = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
	npcInvne->Init();
	npcInvne->SetName("NpcInven");*/

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
	//npc_items = npcInvne->GetItems();
	sell_items = sellInven->GetItems();
	buy_items = buyInven->GetItems();

	enabled = false;

	/*npcInvne->AddItem("Recoverykit", 2);
	npcInvne->AddItem("Recoverykit", 2);
	npcInvne->AddItem("Apple",2);
	npcInvne->AddItem("Water",2);
	npcInvne->AddItem("EnergyDrink",2);
	npcInvne->AddItem("RifleBullet",100);*/

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

	for (auto& sc : shopCategorybtn)
	{
		sc->Update(dt);
	}

	/*for (auto& sb : shopCategoryBox)
	{
		sb->Update(dt);
	}*/

	for (int i = 0; i < shopCategorybtn.size(); i++)
	{
		if (shopCategorybtn[i]->IsClick())
		{
			for (auto& sb : shopCategoryBox)
			{
				sb->SetActive(false);
			}
			shopCategoryBox[i]->SetActive(true);
			//shopCategoryBox[i]->craftingItmeName = "";
		}
		if (!shopCategoryBox[i]->GetActive())
			continue;
	}

	for (auto items : *player_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			cout << "player_items" << endl;
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
		items->ToolTipUpdate(dt);
	}
	for (auto& box : shopCategoryBox)
	{
		if (!box->GetActive())
			continue;
		for (auto items : *box->GetItems())
		{
			items->ButtonUpdate(dt);
			if (items->IsClick())
			{
				cout << "npc_items" << endl;
				auto move_pos = buyInven->FindInvenPos(items->GetWidth(), items->GetHeight());
				if (move_pos != Vector2i{ -1,-1 })
				{
					itemPrevPos[items] = items->GetGreedPos();
					prevItemBox[items] = box;
					box->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });
					buyInven->MoveItem(items, Vector2i{ move_pos.y, move_pos.x });

					price -= items->GetPrice() * items->GetCount();
					txtPrice->SetString("DEAL " + to_string(price));
					txtPrice->SetOrigin(Origins::MC);
				}
				return;
			}
			items->ToolTipUpdate(dt);
		}
	}

	for (auto items : *sell_items)
	{
		items->ButtonUpdate(dt);
		if (items->IsClick())
		{
			cout << "sell_items" << endl;
			sellInven->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });

			auto prev = itemPrevPos[items];

			playerInven->MoveItem(items, prev);
			itemPrevPos.erase(items);

			price -= items->GetPrice() * items->GetCount();
			txtPrice->SetString("DEAL " + to_string(price));
			txtPrice->SetOrigin(Origins::MC);
			return;
		}
		items->ToolTipUpdate(dt);
	}
		for (auto items : *buy_items)
		{
			items->ButtonUpdate(dt);
			if (items->IsClick())
			{
				cout << "buy_items" << endl;
				buyInven->DeleteItem(items, items->GetGreedPos(), Vector2i{ items->GetWidth(),items->GetHeight() });

				auto prev = itemPrevPos[items];
				InventoryBox* box = prevItemBox[items];
				box->MoveItem(items, prev);

				itemPrevPos.erase(items);
				prevItemBox.erase(items);
				price += items->GetPrice() * items->GetCount();
				txtPrice->SetString("DEAL " + to_string(price));
				txtPrice->SetOrigin(Origins::MC);
				return;
			}
			items->ToolTipUpdate(dt);
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
	//npcInvne->Draw(window);
	sellInven->Draw(window);
	buyInven->Draw(window);

	txtMoney->Draw(window);
	dealBtn->Draw(window);
	txtPrice->Draw(window);

	for (auto& btn : shopCategorybtn)
	{
		btn->Draw(window);
	}
	for (auto& cg : shopCategoryBox)
	{
		cg->Draw(window);
	}
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
	for (auto& items : *sell_items)
	{
		auto prev = itemPrevPos[items];
		playerInven->MoveItem(items, prev);
	}
	sell_items->clear();
	for(auto& box : shopCategoryBox)
	{
		for (auto& items : *buy_items)
		{
			auto prev = itemPrevPos[items];
			box->MoveItem(items, prev);
		}
		buy_items->clear();
	}
	//for (auto& items : *buy_items)
	//{
	//	auto prev = itemPrevPos[items];
	//	//npcInvne->MoveItem(items, prev);
	//}
	//buy_items->clear();

	sellInven->Reset();
	buyInven->Reset();

	itemPrevPos.clear();
	prevItemBox.clear();
	price = 0.f;
	txtPrice->SetString("DEAL " + to_string(price));
	txtPrice->SetOrigin(Origins::MC);
}

void Shop::Deal()
{
	if (((Ready*)SCENE_MGR->GetCurrScene())->GetPlayer()->GetMoney() + price < 0)
	{
		return;
	}

	for (auto& items : *sell_items)
	{
		delete items;
	}
	sell_items->clear();
	for (auto& items : *buy_items)
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
