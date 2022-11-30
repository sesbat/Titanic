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

Inventory::Inventory(UiMgr* mgr)
	: Button(mgr), totalWeight(0.f), nowDrag(nullptr), prevInven(nullptr), useIdx(-1)
{
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/inventory.png"), true);
	myInven = new InventoryBox(uimgr, this, Vector2i{ 163,252 });
	myInven->Init();
	myInven->SetName("MyInventory");
	rightInven = new InventoryBox(uimgr, this, Vector2i{ 1248,252 });
	rightInven->Init();
	rightInven->SetName("RightInventory");
	initRightInven = rightInven;


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
}

void Inventory::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	myInven->Update(dt);
	rightInven->Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Q))
	{
		myInven->AddItem("Recoverykit");
	}
	if (InputMgr::GetKeyDown(Keyboard::W))
	{
		myInven->AddItem("handsaw");
	}
	if (InputMgr::GetKeyDown(Keyboard::T))
	{
		myInven->AddItem("Armor-1");
	}
	if (InputMgr::GetKeyDown(Keyboard::E))
	{
		rightInven->AddItem("Recoverykit");
	}
	if (InputMgr::GetKeyDown(Keyboard::R))
	{
		rightInven->AddItem("handsaw");
	}

	if (nowDrag != nullptr && IsStay() )
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
					if (itemName == "GUN1" || 
						itemName == "GUN2")
						isUseItemUp = true;
					break;
				case 1:
					if (itemName == "GUN1" ||
						itemName == "GUN2")
						isUseItemUp = true;
					break;
				case 2:
					if (itemName == "Armor-1")
						isUseItemUp = true;
					break;
				case 3:
					if (itemName == "Armor-1")
						isUseItemUp = true;
					break;
				case 4:
					if (itemName == "Recoverykit")
						isUseItemUp = true;
					break;
				case 5:
					if (itemName == "Recoverykit")
						isUseItemUp = true;
					break;
				case 6:
					if (itemName == "Recoverykit")
						isUseItemUp = true;
					break;
				case 7:
					if (itemName == "Recoverykit")
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
				break;
			}
		}
		i++;
	}
}

void Inventory::Draw(RenderWindow& window)
{
	if (!enabled)
		return;

	Button::Draw(window);
	for (auto& useItem : invenItemGreed)
	{
		if(useItem != nullptr)
			useItem->Draw(window);
	}
	for (auto& useItem : myUseItems)
	{
		if (useItem != nullptr)
			useItem->Draw(window);
	}
	myInven->Draw(window);
	rightInven->Draw(window);

}

void Inventory::SetDrag(InvenItem* item)
{
	nowDrag = item;
	myInven->SetDrag(item);
	rightInven->SetDrag(item);
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
	if(find_item != prev->end())
		prev->erase(find_item);
	next->push_back(nowDrag);
}

void Inventory::ReturnUseItem()
{
	nowDrag->SetInvenPos(invenItemGreed[useIdx]->GetPos());
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
	cout << myInven->GetName() << endl;
	cout << rightInven->GetName() << endl;
	rightInven = initRightInven;
	myInven->SetPair(rightInven);
	rightInven->SetPair(myInven);
}

InvenItem* Inventory::GetUsedItem(int i)
{
	return myUseItems[i];
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