#include "AddItemBox.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/FileManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/InputMgr.h"

AddItemBox::AddItemBox(UiMgr* mgr)
	: Button(mgr), nowObjectItems(nullptr)
{
}

AddItemBox::~AddItemBox()
{
	Release();
}

void AddItemBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/itemBox.png"), true);
	SetClkColor(false);

	SetPos(Vector2f{ 1340, 60 });
	SetActive(false);

	auto& itemData = FILE_MGR->GetAllItems();

	Vector2f pos = { 1400,160 };
	for (auto& data : itemData)
	{
		AddItem* aitem = new AddItem(uimgr);
		aitem->item = { data.first,data.second.path,0 };
		aitem->addCnt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			30, Color::White, ">", true);
		aitem->minCnt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			30, Color::White, "<", true);
		aitem->sprite->SetTexture(*RESOURCES_MGR->GetTexture(data.second.path));
		aitem->text->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			30, Color::White, to_string(aitem->item.count));
		aitem->SetPosition(pos);
		pos.y += 200.f;
		allItems[data.first] = aitem;
	}
}

void AddItemBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);

	if (InputMgr::GetMouseWheelUp())
	{
		addItemIdx--;
		addItemIdx = max(addItemIdx, 0);

		Vector2f pos = { 1400,160 };
		int i = 0;
		for (auto& item : allItems)
		{
			if (i >= addItemIdx && i < addItemIdx + 4)
			{
				item.second->SetPosition(pos);
				pos.y += 200.f;
			}
			i++;
		}
	}
	if (InputMgr::GetMouseWheelDown())
	{
		addItemIdx++;
		addItemIdx = min((int)allItems.size(), addItemIdx);

		Vector2f pos = { 1400,160 };
		int i = 0;
		for (auto& item : allItems)
		{
			if (i >= addItemIdx && i < addItemIdx + 4)
			{
				item.second->SetPosition(pos);
				pos.y += 200.f;
			}
			i++;
		}
	}

	int i = 0;
	for (auto& item : allItems)
	{
		if (i >= addItemIdx && i < addItemIdx + 4)
		{
			item.second->Update(dt);

			auto& now = item.second;

			if (now->addCnt->IsUp())
			{
				now->item.count++;
				auto find_item = nowObjectItems->find(now->item.type);

				if (find_item == nowObjectItems->end())
				{
					(*nowObjectItems)[now->item.type].path = now->item.path;
					(*nowObjectItems)[now->item.type].type = now->item.type;
					(*nowObjectItems)[now->item.type].count = 1;
				}
				else
				{
					(*find_item).second.count++;
					(*nowObjectItems)[now->item.type].count = (*find_item).second.count;
				}
				now->text->SetString(to_string(now->item.count));
			}
			if (now->minCnt->IsUp())
			{
				now->item.count--;
				now->item.count = max(0, now->item.count);

				auto find_item = nowObjectItems->find(now->item.type);
				if (find_item == nowObjectItems->end())
					return;
				else
				{
					if (now->item.count == 0)
						nowObjectItems->erase(find_item);
					else
					{
						(*find_item).second.count--;
						(*nowObjectItems)[now->item.type].count = (*find_item).second.count;
					}
				}
				now->text->SetString(to_string(now->item.count));
			}
		}
		i++;

	}


}

void AddItemBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	int i = 0;
	for (auto& item : allItems)
	{
		if (i >= addItemIdx && i < addItemIdx + 4)
		{
			item.second->Draw(window);
		}
		i++;

	}
}

void AddItemBox::Release()
{
	for (auto item : allItems)
	{
		delete item.second;
	}
	allItems.clear();
}

void AddItemBox::SetItems(map<string,Item>& items)
{
	nowObjectItems = &items;

	if (enabled)
	for (auto& data : allItems)
	{
		data.second->item.count = 0;

		if ((*nowObjectItems).find(data.first) != nowObjectItems->end())
			data.second->item.count = (*nowObjectItems)[data.first].count;

		data.second->text->SetString(to_string(data.second->item.count));
	}
}

AddItem::AddItem(UiMgr* mgr)
{
	sprite = new SpriteObject();
	sprite->SetUI(true);
	sprite->SetOrigin(Origins::TC);
	addCnt = new Button(mgr);
	addCnt->SetOrigin(Origins::MC);
	addCnt->SetClkColor(true);
	minCnt = new Button(mgr);
	minCnt->SetOrigin(Origins::MC);
	minCnt->SetClkColor(true);
	text = new TextObject();
	text->SetOrigin(Origins::MC);
	item.count = 0;
}

AddItem::~AddItem()
{
	if (sprite != nullptr)
		delete sprite;
	sprite = nullptr;

	if (text != nullptr)
		delete text;
	text = nullptr;

	if (addCnt != nullptr)
		delete addCnt;
	addCnt = nullptr;

	if (minCnt != nullptr)
		delete minCnt;
	minCnt = nullptr;
}

void AddItem::SetPosition(Vector2f pos)
{
	sprite->SetPos(pos);
	minCnt->SetPos(pos + Vector2f{ 200.f,0 });
	text->SetPos(pos + Vector2f{ 250.f,0 });
	addCnt->SetPos(pos + Vector2f{ 300.f,0 });
}

void AddItem::Update(float dt)
{
	minCnt->Update(dt);
	addCnt->Update(dt);
}

void AddItem::Draw(RenderWindow& window)
{
	sprite->Draw(window);
	minCnt->Draw(window);
	text->Draw(window);
	addCnt->Draw(window);
}
