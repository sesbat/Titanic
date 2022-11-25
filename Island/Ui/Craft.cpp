#include "Craft.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "CraftGreed.h"
#include "InvenItem.h"
#include "CraftBox.h"
#include "../Framework/info.h"
#include "../Framework/Framework.h"
#include "Button.h"

Craft::Craft(UiMgr* mgr)
	: Button(mgr), totalWeight(0.f), nowDrag(nullptr), prevCraft(nullptr), useIdx(-1),categoryCount(3)
{
}

Craft::~Craft()
{
}

void Craft::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/craftbk.png"), true);
	craftBox = new CraftBox(uimgr, this, Vector2i{ 512,64 });
	craftBox->Init();
	craftBox->SetName("MyInventory");

	requiredItem = new CraftBox(uimgr, this, Vector2i{ 512,704 });
	requiredItem->SetBoxSize(10,4);
	requiredItem->Init();
	requiredItem->SetName("RightInventory");
	initRequiredItem = requiredItem;

	for (int i = 0; i < 3; i++)
	{
		Button* button = new Button(uimgr);
		button->SetClkColor(true);
		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			50, Color::White, " Health", true);
		button->SetOrigin(Origins::MC);
		button->SetPos({ 300.f,200.f + FRAMEWORK->GetWindowSize().y/3 *i});
	}

	/*Vector2f invenPos[] = { {504.f,56.f},{504.f,696.f} };
	string invenPath[] = { "craft-buy","craft-sel"};

	for (int i = 0; i < 8; i++)
	{
		craftGreed[i] = new Button(uimgr);
		craftGreed[i]->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/" + invenPath[i] + ".png"), true);
		craftGreed[i]->SetPos(invenPos[i]);
		craftGreed[i]->SetClkColor(false);
	}*/
	myUseItems = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void Craft::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	craftBox->Update(dt);
	requiredItem->Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Q))
	{
		craftBox->AddItem("Recoverykit");
	}
	if (InputMgr::GetKeyDown(Keyboard::W))
	{
		craftBox->AddItem("handsaw");
	}
	if (InputMgr::GetKeyDown(Keyboard::T))
	{
		craftBox->AddItem("Armor-1");
	}
	if (InputMgr::GetKeyDown(Keyboard::E))
	{
		requiredItem->AddItem("Recoverykit");
	}
	if (InputMgr::GetKeyDown(Keyboard::R))
	{
		requiredItem->AddItem("handsaw");
	}

	if (nowDrag != nullptr && IsStay())
	{
		prevCraft->ReturnItem();
	}
	int i = 0;
	for (auto& useItem : craftGreed)
	{
		if (useItem == nullptr)
			continue;
		useItem->Update(dt);

		if (useItem->IsUp())
		{
			if (nowDrag != nullptr && myUseItems[i] == nullptr)
			{
				string itemName = nowDrag->GetName();
				if (itemName == "Recoverykit")
				{
					if (!(i > 3 && i < 8))
					{
						prevCraft->ReturnItem();
						break;
					}
				}
				if (itemName == "Armor-1")
				{
					if (i != 2)
					{
						prevCraft->ReturnItem();
						break;
					}
				}
				if (itemName == "handsaw")
				{
					break;
				}
				//nowDrag->SetPos(useItem->GetPos());
				nowDrag->SetInvenPos(useItem->GetPos());
				auto item = prevCraft->GetItems();

				auto find_item = find(item->begin(), item->end(), nowDrag);
				if (find_item != item->end())
					item->erase(find_item);

				myUseItems[i] = nowDrag;
				SetDrag(nullptr);
				useIdx = -1;
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
				prevCraft = craftBox;
				SetPrevInven(craftBox);
				useItem = nullptr;
				useIdx = i;
				break;
			}
		}
		i++;
	}
}

void Craft::Draw(RenderWindow& window)
{
	if (!enabled)
		return;

	Button::Draw(window);
	for (auto& useItem : craftGreed)
	{
		if (useItem != nullptr)
			useItem->Draw(window);
	}
	for (auto& useItem : myUseItems)
	{
		if (useItem != nullptr)
			useItem->Draw(window);
	}
	craftBox->Draw(window);
	requiredItem->Draw(window);
	for (auto& cg : categories)
	{
		cg->Draw(window);
	}

}

void Craft::SetDrag(InvenItem* item)
{
	nowDrag = item;
	craftBox->SetDrag(item);
	requiredItem->SetDrag(item);
}
CraftBox* Craft::GetNowInven()
{
	return (InputMgr::GetMousePos().x <= WINDOW_WIDTH / 2) ? craftBox : requiredItem;
}
CraftBox* Craft::GetPrevInven()
{
	return prevCraft;
}

CraftBox* Craft::GetRightInven()
{
	return requiredItem;
}
CraftBox* Craft::GetPairBox(CraftBox* now)
{
	return now == craftBox ? requiredItem : craftBox;
}


void Craft::MoveInvenItem(CraftBox* nextInven)
{
	if (prevCraft == nextInven && useIdx == -1)
	{
		return;
	}

	auto prev = prevCraft->GetItems();
	auto next = nextInven->GetItems();

	auto find_item = find(prev->begin(), prev->end(), nowDrag);
	if (find_item != prev->end())
		prev->erase(find_item);
	next->push_back(nowDrag);
}

void Craft::ReturnUseItem()
{
	nowDrag->SetInvenPos(craftGreed[useIdx]->GetPos());
	myUseItems[useIdx] = nowDrag;
	SetDrag(nullptr);
	SetPrevInven(nullptr);
	useIdx = -1;
}

void Craft::ClearInven()
{
	initRequiredItem->ClearInven();
}

void Craft::ResetRightInven()
{
	cout << craftBox->GetName() << endl;
	cout << requiredItem->GetName() << endl;
	requiredItem = initRequiredItem;
	craftBox->SetPair(requiredItem);
	requiredItem->SetPair(craftBox);
}

CraftGreed* Craft::GetGreed(int i, int j)
{
	CraftBox* inven = GetNowInven();

	int	width = inven->GetWidth();
	int	height = inven->GetHeight();
	if (i >= height || j >= width)
		return nullptr;

	return inven->GetGreed(i, j);
}