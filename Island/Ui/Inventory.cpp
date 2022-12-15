#include "Inventory.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "InvenGreed.h"
#include "InvenItem.h"
#include "InventoryBox.h"
#include "../Framework/info.h"
#include "../GameObject/Player.h"
#include "../Scens/SceneManager.h"
#include "../Scens/GameScene.h"
#include "../GameObject/Gun.h"

Inventory::Inventory(UiMgr* mgr)
	: Button(mgr), totalWeight(0.f), nowDrag(nullptr), prevInven(nullptr), useIdx(-1)
{
}

Inventory::~Inventory()
{
	Release();
}

void Inventory::Release()
{
	for (auto& useItem : invenItemGreed)
	{
		if (useItem != nullptr)
		{
			delete useItem;
			useItem = nullptr;
		}
	}
	for (auto& useItem : myUseItems)
	{
		delete useItem;
	}
	myUseItems.clear();

	if (myInven != nullptr)
		delete myInven;
	myInven = nullptr;

	if (rightInven != nullptr)
		delete rightInven;
	rightInven = nullptr;

	if (txtMoney != nullptr)
		delete txtMoney;
	txtMoney = nullptr;

	Button::Release();
}

void Inventory::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/inventory.png"), true);
	myInven = new InventoryBox(uimgr, this, Vector2i{ 163,252 });
	myInven->SetName("MyInventory");
	myInven->Init();
	rightInven = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
	rightInven->SetName("RightInventory");
	rightInven->Init();
	initRightInven = rightInven;

	saveBox = new InventoryBox(uimgr, this, Vector2i{ 870, 845 });
	saveBox->SetInvenSize(3, 2);
	saveBox->SetName("SaveBox");
	saveBox->Init();
	saveBox->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		30, Color::White, "SAVE BOX", false);
	saveBox->GetTextObj()->SetPos(Vector2f{ 880.f, 800.f });


	Vector2f invenPos[] = { {800.f,140.f},{800.f,310.f},{800.f,471.f},{991.f,471.f},{800.f,721.f},{888.f,721.f},{972.f,721.f},{1055.f,721.f} };
	string invenPath[] = { "inven-weapon","inven-weapon" ,"inven-cloth" ,"inven-cloth" ,"inven-item","inven-item" ,"inven-item" ,"inven-item" };

	for (int i = 0; i < 8; i++)
	{
		invenItemGreed[i] = new Button(uimgr);
		invenItemGreed[i]->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/" + invenPath[i] + ".png"), true);
		invenItemGreed[i]->SetPos(invenPos[i]);
		invenItemGreed[i]->SetClkColor(false);
	}
	myUseItems = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	txtMoney = new TextObject();
	txtMoney->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 20, Color::White, to_string(((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer()->GetMoney()));
	txtMoney->SetPos(Vector2f{ 660.f,990.f });
	txtMoney->SetOrigin(Origins::MR);
}

void Inventory::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	myInven->Update(dt);
	rightInven->Update(dt);
	saveBox->Update(dt);


	if (nowDrag != nullptr && IsStay())
	{
		prevInven->ReturnItem();
	}
	int i = 0;
	bool isUseItemUp = false;
	for (auto& useItem : invenItemGreed)
	{
		if (useItem == nullptr)
			continue;
		useItem->Update(dt);

		if (nowDrag != nullptr && useItem->IsUp())
		{
			string itemName = nowDrag->GetName();
			if (nowDrag != nullptr && myUseItems[i] == nullptr)
			{
				switch (i)
				{
				case 0:
					if (itemName == "Shotgun" ||
						itemName == "MB_Shotgun" ||
						itemName == "Up1-Shotgun" ||
						itemName == "Rifle" ||
						itemName == "Scop_Rifle" ||
						itemName == "MB_Rifle" ||
						itemName == "Sniper" ||
						itemName == "Scop_sniper" ||
						itemName == "MB_sniper"||
						itemName == "SR_25")
						isUseItemUp = true;
					break;
				case 1:
					if (itemName == "Shotgun" ||
						itemName == "MB_Shotgun" ||
						itemName == "Up1-Shotgun" ||
						itemName == "Rifle" ||
						itemName == "Scop_Rifle" ||
						itemName == "MB_Rifle" ||
						itemName == "Sniper" ||
						itemName == "Scop_sniper" ||
						itemName == "MB_sniper" ||
						itemName == "SR_25")
						isUseItemUp = true;
					break;
				case 2:
					if (itemName == "Armor-1"||
						itemName == "Armor-2" || 
						itemName == "Armor-3" || 
						itemName == "Armor-4")
						isUseItemUp = true;
					break;
				case 3:
					if (itemName == "Armor-1" ||
						itemName == "Armor-2" ||
						itemName == "Armor-3" ||
						itemName == "Armor-4")
						isUseItemUp = true;
					break;
				case 4:
					if (itemName == "Recoverykit" ||
						itemName == "Apple" ||
						itemName == "Meat" ||
						itemName == "Water" ||
						itemName == "EnergyDrink"||
						itemName == "Bread" || 
						itemName == "RottenBread" || 
						itemName == "Cheese" || 
						itemName == "RottenCheese" || 
						itemName == "Egg" || 
						itemName == "RottenEgg" || 
						itemName == "Sandwitch" || 
						itemName == "MRE" || 
						itemName == "RottenWater" || 
						itemName == "Milk" || 
						itemName == "RottenMilk" || 
						itemName == "Coke" || 
						itemName == "Coffee" || 
						itemName == "Vodka" ||
						itemName == "Whisky" || 
						itemName == "AntiRad" || 
						itemName == "RawMeat" || 
						itemName == "StandardMedikit" || 
						itemName == "ModernMedikit" )
						isUseItemUp = true;
					break;
				case 5:
					if (itemName == "Recoverykit" ||
						itemName == "Apple" ||
						itemName == "Meat" ||
						itemName == "Water" ||
						itemName == "EnergyDrink" ||
						itemName == "Bread" ||
						itemName == "RottenBread" ||
						itemName == "Cheese" ||
						itemName == "RottenCheese" ||
						itemName == "Egg" ||
						itemName == "RottenEgg" ||
						itemName == "Sandwitch" ||
						itemName == "MRE" ||
						itemName == "RottenWater" ||
						itemName == "Milk" ||
						itemName == "RottenMilk" ||
						itemName == "Coke" ||
						itemName == "Coffee" ||
						itemName == "Vodka" ||
						itemName == "Whisky" ||
						itemName == "AntiRad" ||
						itemName == "RawMeat" ||
						itemName == "StandardMedikit" ||
						itemName == "ModernMedikit")
						isUseItemUp = true;
					break;
				case 6:
					if (itemName == "Recoverykit" ||
						itemName == "Apple" ||
						itemName == "Meat" ||
						itemName == "Water" ||
						itemName == "EnergyDrink" ||
						itemName == "Bread" ||
						itemName == "RottenBread" ||
						itemName == "Cheese" ||
						itemName == "RottenCheese" ||
						itemName == "Egg" ||
						itemName == "RottenEgg" ||
						itemName == "Sandwitch" ||
						itemName == "MRE" ||
						itemName == "RottenWater" ||
						itemName == "Milk" ||
						itemName == "RottenMilk" ||
						itemName == "Coke" ||
						itemName == "Coffee" ||
						itemName == "Vodka" ||
						itemName == "Whisky" ||
						itemName == "AntiRad" ||
						itemName == "RawMeat" ||
						itemName == "StandardMedikit" ||
						itemName == "ModernMedikit")
						isUseItemUp = true;
					break;
				case 7:
					if (itemName == "Recoverykit" ||
						itemName == "Apple" ||
						itemName == "Meat" ||
						itemName == "Water" ||
						itemName == "EnergyDrink" ||
						itemName == "Bread" ||
						itemName == "RottenBread" ||
						itemName == "Cheese" ||
						itemName == "RottenCheese" ||
						itemName == "Egg" ||
						itemName == "RottenEgg" ||
						itemName == "Sandwitch" ||
						itemName == "MRE" ||
						itemName == "RottenWater" ||
						itemName == "Milk" ||
						itemName == "RottenMilk" ||
						itemName == "Coke" ||
						itemName == "Coffee" ||
						itemName == "Vodka" ||
						itemName == "Whisky" ||
						itemName == "AntiRad" ||
						itemName == "RawMeat" ||
						itemName == "StandardMedikit" ||
						itemName == "ModernMedikit")
						isUseItemUp = true;
					break;
				}

				if (isUseItemUp)
				{
					nowDrag->SetInvenPos(useItem->GetPos());
					auto item = prevInven->GetItems();

					auto find_item = find(item->begin(), item->end(), nowDrag);
					if (find_item != item->end())
						item->erase(find_item);

					myUseItems[i] = nowDrag;
					if (i == 0 || i == 1)
					{
						auto player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
						auto gun = player->GetGun();

						gun->SetGunType(gun->ItemNameToType(nowDrag->GetName()));
					}
					SetDrag(nullptr);
					useIdx = -1;
				}

			}
		}
		i++;
	}
	i = 0;
	for (auto& useItem : myUseItems)
	{
		if (nowDrag != nullptr)
			break;
		if (useItem != nullptr)
		{
			useItem->Update(dt);
			if (useItem->IsDown())
			{
				SetDrag(useItem);
				prevInven = myInven;
				SetPrevInven(myInven);
				useItem = nullptr;
				useIdx = i;

				if (i == 0 || i == 1)
				{
					auto player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
					auto gun = player->GetGun();

					gun->SetGunType(GunType::None);
				}
				break;
			}
		}
		i++;
	}

	for (auto& del : deleteUseItem)
	{
		int idx = del - myUseItems.begin();
		delete* del;
		myUseItems[idx] = nullptr;
	}
	deleteUseItem.clear();
}

void Inventory::Draw(RenderWindow& window)
{
	if (!enabled)
		return;

	Button::Draw(window);
	for (auto& useItem : invenItemGreed)
	{
		if (useItem != nullptr)
			useItem->Draw(window);
	}
	for (auto& useItem : myUseItems)
	{
		if (useItem != nullptr)
			useItem->Draw(window);
	}
	myInven->Draw(window);
	rightInven->Draw(window);
	txtMoney->Draw(window);
	saveBox->Draw(window);


}

void Inventory::SetDrag(InvenItem* item)
{
	nowDrag = item;
	myInven->SetDrag(item);
	rightInven->SetDrag(item);
	saveBox->SetDrag(item);
}
InventoryBox* Inventory::GetNowInven()
{
	return (InputMgr::GetMousePos().x <= WINDOW_WIDTH / 2) ? myInven : rightInven;
}
InventoryBox* Inventory::GetPrevInven()
{
	return prevInven;
}

InventoryBox* Inventory::GetRightInven()
{
	return rightInven;
}
InventoryBox* Inventory::GetPairBox(InventoryBox* now)
{
	return now == myInven ? rightInven : myInven;
}


void Inventory::MoveInvenItem(InventoryBox* nextInven)
{
	if (prevInven == nextInven && useIdx == -1)
	{
		return;
	}

	auto prev = prevInven->GetItems();
	auto next = nextInven->GetItems();

	auto find_item = find(prev->begin(), prev->end(), nowDrag);
	if (find_item != prev->end())
		prev->erase(find_item);
	next->push_back(nowDrag);
}

void Inventory::ReturnUseItem()
{
	nowDrag->SetInvenPos(invenItemGreed[useIdx]->GetPos());

	if (useIdx == 0 || useIdx == 1)
	{
		auto player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
		auto gun = player->GetGun();

		gun->SetGunType(gun->ItemNameToType(nowDrag->GetName()));
	}
	myUseItems[useIdx] = nowDrag;
	SetDrag(nullptr);
	SetPrevInven(nullptr);
	useIdx = -1;
}

void Inventory::ClearInven()
{
	initRightInven->ClearInven();
}

void Inventory::ResetRightInven()
{
	//cout << myInven->GetName() << endl;
	//cout << rightInven->GetName() << endl;
	rightInven = initRightInven;
	myInven->SetPair(rightInven);
	rightInven->SetPair(myInven);
}

InvenItem* Inventory::GetUsedItem(int i)
{
	return myUseItems[i];
}

void Inventory::AddDeleteObj(InvenItem* obj)
{
	auto it = find(myUseItems.begin(), myUseItems.end(), obj);
	deleteUseItem.push_back(it);
	myUseItems[it - myUseItems.begin()] = nullptr;
}

void Inventory::AddDeleteItem(InvenItem* item)
{
	myInven->DeleteItem(item);
}

void Inventory::SetUserItem(InvneUseInfo data)
{
	auto itemData = FILE_MGR->GetItemInfo(data.Type);
	this->myUseItems[data.useIdx] = new InvenItem(uimgr);
	this->myUseItems[data.useIdx]->SetName(data.Type);
	this->myUseItems[data.useIdx]->Set(itemData.width, itemData.height, data.invenPos, { -1, -1 }, data.path, itemData.maxCount, itemData.price);
	this->myUseItems[data.useIdx]->Init();
	this->myUseItems[data.useIdx]->AddCount(data.cnt);

	if (data.useIdx == 0 || data.useIdx == 1)
	{
		auto player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
		auto gun = player->GetGun();

		gun->SetGunType(gun->ItemNameToType(data.Type));
	}
}

InvenGreed* Inventory::GetGreed(int i, int j)
{
	InventoryBox* inven = GetNowInven();

	int	width = inven->GetWidth();
	int	height = inven->GetHeight();
	if (i >= height || j >= width)
		return nullptr;

	return inven->GetGreed(i, j);
}