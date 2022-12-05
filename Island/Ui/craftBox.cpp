#include "craftBox.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "../Scens/SceneManager.h"
#include "../Scens/GameScene.h"
#include "InvenItem.h"
#include "Craft.h"
#include "CraftItem.h"
#include "../GameObject/Player.h"
#include "Inventory.h"
#include "InventoryBox.h"

CraftBox::CraftBox(UiMgr* mgr, Craft* inven, Vector2i startPos)
	: Button(mgr),inven(inven), startPos(startPos)
{
}

CraftBox::~CraftBox()
{
	Release();
}

void CraftBox::Init()
{
	for (int i = 0; i < height; i++)
	{
		vector<bool> line;
		vector<CraftGreed*> greedLine;
		for (int j = 0; j < width; j++)
		{
			line.push_back(false);

			CraftGreed* greed = new CraftGreed(uimgr, inven, this);
			greed->Init();
			greed->SetPos({ startPos.x + j * 60.f + padding * j , startPos.y + i * 60.f + padding * i });
			greed->SetInvenPos(j, i);
			greedLine.push_back(greed);
		}
		allPos.push_back(line);
		itemGreed.push_back(greedLine);
	}
	player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
}

void CraftBox::Update(float dt)
{
	if (!enabled)
		return;

	Button::Update(dt);

	for (auto& greedLine : itemGreed)
	{
		for (auto& greed : greedLine)
		{
			greed->Update(dt);
			for (auto& n : items)
			{
				n->Update(dt);
				if (!isRequired && n->IsClick())
				{
					inven->OnClickCraftItem(FILE_MGR->GetCraftItemInfo(n->GetName()));
					craftingItmeName = n->GetName();
				}
			}
		}
	}

	if (!isRequired && InputMgr::GetKeyDown(Keyboard::Space))
	{
		if (Crafting(craftingItmeName))
		{
			player->GetInventory()->GetPlayerInven()->AddItem(craftingItmeName);
			DeletePlayerItem(craftingItmeName);
		}
	}
}
bool CraftBox::Crafting(string itemName)
{
	if (itemName == "")
		return false;
	auto data = FILE_MGR->GetCraftItemInfo(itemName);
	auto craftUseItem = data.useItem;
	vector<bool> isItems;
	auto playerItems = *player->GetInventory()->GetPlayerInven()->GetItems();
	for (auto i : craftUseItem)
	{
		auto name = i.id;
		auto cnt = i.cnt;
		bool isItem = false;
		for (auto item : playerItems)
		{
			if (item->GetName() == name)
			{
				isItem = true;
				cnt -= item->GetCount();

				if (cnt <= 0)
				{
					isItems.push_back(true);
					break;
				}
			}
		}
		if (!isItem)
			return false;
	}

	for (auto test : isItems)
	{
		if (!test)
			return false;
	}
	return true;
}
void CraftBox::DeletePlayerItem(string itemName)
{
	auto data = FILE_MGR->GetCraftItemInfo(itemName);
	auto craftUseItem = data.useItem;
	auto playerItems = player->GetInventory()->GetPlayerInven()->GetItems();
	auto playerInven = player->GetInventory();
	auto playerInvenBox = player->GetInventory()->GetPlayerInven();
	for (auto i : craftUseItem)
	{
		auto name = i.id;
		auto cnt = i.cnt;
		for (auto item : *playerItems)
		{
			if (item->GetName() == name)
			{
				auto tmpCnt = cnt > item->GetCount() ? item->GetCount() : cnt;
				
				playerInvenBox->AddItem(name, -tmpCnt);
				if (item->GetCount() <= 0)
				{
					playerInven->AddDeleteItem(item);
				}
				cnt -= tmpCnt;

				if(cnt <= 0)
					break;
			}
		}
	}
}
void CraftBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	for (auto& greedLine : itemGreed)
	{
		for (auto& greed : greedLine)
		{
			greed->Draw(window);
		}
	}
	for (auto& item : items)
	{
		item->Draw(window);
	}
}

void CraftBox::AddItem(string name, int count)
{
	auto data = FILE_MGR->GetItemInfo(name);

	for (auto& item : items)
	{
		if (item->GetName() == name)
		{
			if (item->GetCount() + count <= data.maxCount)
			{
				item->AddCount(count);
				//totalWeight += data.weight;
				return;
			}
			else if (item->GetCount() + count > data.maxCount)
			{
				int addCount = data.maxCount - item->GetCount();
				item->AddCount(addCount);
				count -= addCount;
			}
		}
	}

	auto findPos = FindInvenPos(data.width, data.height);

	if (findPos == Vector2i{ -1, -1 })
	{
		return;
	}

	int maxCnt = data.maxCount;

	CraftItem* item = new CraftItem(uimgr);
	for (int i = findPos.x; i < findPos.x + data.height; i++)
	{
		for (int j = findPos.y; j < findPos.y + data.width; j++)
		{
			allPos[i][j] = true;
			itemGreed[i][j]->SetState(true, item);

		}
	}
	item->Set(data.width, data.height,
		{ startPos.x + findPos.y * 60 + padding * findPos.y , startPos.y + findPos.x * 60 + padding * findPos.x },
		{ findPos.y, findPos.x }, data.path, data.maxCount);
	item->AddCount(count);
	item->SetName(name);
	item->Init();

	items.push_back(item);
	inven->SetDrag(nullptr);
}

Vector2i CraftBox::FindInvenPos(int i_width, int i_height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (allPos[i][j])
			{
				continue;
			}

			bool isCheck = true;
			for (int n_i = i; n_i < i + i_height; n_i++)
			{
				if (i + i_height > height)
				{
					isCheck = false;
					break;
				}
				if (!isCheck)
				{
					break;
				}
				for (int n_j = j; n_j < j + i_width; n_j++)
				{
					if (allPos[n_i][n_j] || j + i_width > width)
					{
						isCheck = false;
						break;
					}
				}
			}
			if (isCheck)
			{
				return Vector2i{ i,j };
			}
		}
	}
	return Vector2i{ -1,-1 };
}

CraftGreed* CraftBox::GetGreed(int i, int j)
{
	if (i >= height || j >= width)
		return nullptr;

	return itemGreed[i][j];
}


void CraftBox::SetBoxSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void CraftBox::ClearInven()
{
	for (auto item : items)
	{
		delete item;
	}

	items.clear();

	for (auto& greed_line : itemGreed)
	{
		for (auto greed : greed_line)
		{
			greed->SetState(false, nullptr);
		}
	}
	for (auto& pos_line : allPos)
	{
		for (auto pos : pos_line)
		{
			pos = false;
		}
	}
}

