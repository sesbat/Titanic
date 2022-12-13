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
#include "CraftItem.h"
#include "../Scens/SceneManager.h"
#include "../Scens/Ready.h"
#include "../GameObject/Player.h"

Craft::Craft(UiMgr* mgr)
	: Button(mgr), nowDrag(nullptr), prevCraft(nullptr), categoryCount(3)
{
}

Craft::~Craft()
{
	Release();
}

void Craft::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/craftbk.png"), true);
	player = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetPlayer();

	String itemList[] = { "1", "2", "3" };

	for (int i = 0; i < 3; i++)
	{
		Button* button = new Button(uimgr);
		button->SetClkColor(true);
		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			50, Color::White, (string)itemList[i], true);
		button->SetOrigin(Origins::MC);
		button->SetPos({ 300.f,100.f + FRAMEWORK->GetWindowSize().y/6 *i});
		categories.push_back(button);
	}

	String box[] = { "box1", "box2", "box3" };
	for (int i = 0; i < 3; i++)
	{
		craftBox = new CraftBox(uimgr, this, Vector2i{ 512,64 });
		craftBox->SetClkColor(true);
		craftBox->Init();
		craftBox->SetName(box[i]);
		craftBox->SetActive(false);
		categoryBox.push_back(craftBox);
	}

	requiredItem = new CraftBox(uimgr, this, Vector2i{ 512,704 });
	requiredItem->isRequired = true;
	requiredItem->SetBoxSize(10, 4);
	requiredItem->Init();
	initRequiredItem = requiredItem;

	auto& map = FILE_MGR->GetAllCraftTable();

	int i = 0;
	for (auto& craftPage : map)
	{
		for(auto& craftItem : craftPage)
		{
			categoryBox[i]->AddItem(craftItem.first);
		}
		i++;
	}

	categoryBox[0]->SetActive(true);
}

void Craft::Update(float dt)
{
	if (!enabled)
		return;

	for (auto& cg : categories)
	{
		cg->Update(dt);
	}

	for (auto& btn : categoryBox)
	{
		btn->Update(dt);
	}
	requiredItem->Update(dt);

	for (int i = 0; i < categories.size(); i++)
	{
		if (categories[i]->IsClick())
		{
			for (auto& btn : categoryBox)
			{
				btn->SetActive(false);
			}
			categoryBox[i]->SetActive(true);
			categoryBox[i]->craftingItmeName = "";
		}
		if (!categoryBox[i]->GetActive())
			continue;
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

	for (auto& btn : categoryBox)
	{
		btn->Draw(window);
	}
	requiredItem->Draw(window);
	for (auto& cg : categories)
	{
		cg->Draw(window);
	}
}

void Craft::SetDrag(CraftItem* item)
{
	nowDrag = item;
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
	auto prev = prevCraft->GetItems();
	auto next = nextInven->GetItems();

	auto find_item = find(prev->begin(), prev->end(), nowDrag);
	if (find_item != prev->end())
		prev->erase(find_item);
	next->push_back(nowDrag);
}

void Craft::ClearInven()
{
	initRequiredItem->ClearInven();
}

void Craft::ResetRightInven()
{
	requiredItem = initRequiredItem;
}

void Craft::OnClickCraftItem(const CraftingInfo& info)
{
	requiredItem->ClearInven();
	for (auto& n : info.useItem)
	{
		requiredItem->AddItem(n.id, n.cnt);
	}
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