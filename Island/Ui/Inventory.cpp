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
	: Button(mgr), totalWeight(0.f), nowDrag(nullptr), prevInven(nullptr)
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
}

void Inventory::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
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

InventoryBox* Inventory::GetPairBox(InventoryBox* now)
{
	return now == myInven ? rightInven : myInven;
}


void Inventory::MoveInvenItem(InventoryBox* nextInven)
{
	if (prevInven == nextInven)
		return;

	auto prev = prevInven->GetItems();
	auto next = nextInven->GetItems();

	prev->erase(find(prev->begin(), prev->end(), nowDrag));
	next->push_back(nowDrag);
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