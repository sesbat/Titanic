#include "InventoryBox.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "InvenItem.h"
#include "Inventory.h"

InventoryBox::InventoryBox(UiMgr* mgr, Inventory* inven, Vector2i startPos)
	: Button(mgr),  nowDrag(nullptr), inven(inven), startPos(startPos)
{
}

InventoryBox::~InventoryBox()
{
	Release();
}

void InventoryBox::Init()
{
	for (int i = 0; i < height; i++)
	{
		vector<bool> line;
		vector<InvenGreed*> greedLine;
		for (int j = 0; j < width; j++)
		{
			line.push_back(false);

			InvenGreed* greed = new InvenGreed(uimgr, inven, this);
			greed->Init();
			greed->SetPos({ startPos.x + j * 60.f + padding * j , startPos.y + i * 60.f + padding * i });
			greed->SetInvenPos(j, i);
			greedLine.push_back(greed);
		}
		allPos.push_back(line);
		itemGreed.push_back(greedLine);
	}
}

void InventoryBox::Release()
{
	for (auto& greeds : itemGreed)
	{
		for (auto& greed : greeds)
		{
			if (greed != nullptr)
				delete greed;
			greed = nullptr;
		}
	}

	itemGreed.clear();

	for (auto& item : items)
	{
		if (item != nullptr)
			delete item;
		item = nullptr;
	}
	items.clear();
	allPos.clear();

	Button::Release();
}

void InventoryBox::Update(float dt)
{
	if (!enabled)
		return;

	if(nowDrag == nullptr)
	{
		for (auto& item : items)
		{
			item->Update(dt);
			if (item->IsDown())
			{
				inven->SetDrag(item);
				inven->SetPrevInven(this);
				auto invenPos = nowDrag->GetGreedPos();

				for (int i = invenPos.x; i < invenPos.x + nowDrag->GetWidth(); i++)
				{
					for (int j = invenPos.y; j < invenPos.y + nowDrag->GetHeight(); j++)
					{
						allPos[j][i] = false;
						itemGreed[j][i]->SetState(false, nullptr);
					}
				}
				//items.erase(items.begin(), find(items.begin(), items.end(), item));
				//items.push_back(item);
				break;
			}
			else if (item->IsClickRight())
			{
				auto pos = pairInven->FindInvenPos(item->GetWidth(), item->GetHeight());
				auto invenPos = item->GetGreedPos();


				if (pos != Vector2i{ -1,-1 })
				{
					for (int i = invenPos.x; i < invenPos.x + item->GetWidth(); i++)
					{
						for (int j = invenPos.y; j < invenPos.y + item->GetHeight(); j++)
						{
							allPos[j][i] = false;
							itemGreed[j][i]->SetState(false, nullptr);
						}
					}

					pairInven->AddItem(item->GetName(), item->GetCount());
					items.erase(find(items.begin(), items.end(), item));
					break;
				}
				break;
			}
		}
	}
	else
	{
		nowDrag->Update(dt);
	}

	Button::Update(dt);

	for (auto& greedLine : itemGreed)
	{
		for (auto& greed : greedLine)
		{
			greed->Update(dt);
		}
	}

}

void InventoryBox::Draw(RenderWindow& window)
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
		if (item == nowDrag)
			continue;
		item->Draw(window);
	}
	if (nowDrag != nullptr)
		nowDrag->Draw(window);
}

void InventoryBox::AddItem(string name, int cnt, Vector2i invenPos, Vector2i greedPos, string path)
{
	auto data = FILE_MGR->GetItemInfo(name);

	InvenItem* item = new InvenItem(uimgr);
	item->Init();
	Vector2i findPos;
	findPos.x = greedPos.y;
	findPos.y = greedPos.x;
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
		{ findPos.y, findPos.x }, data.path, data.maxCount, data.price);
	item->AddCount(cnt);
	item->SetName(name);
	item->Init();

	items.push_back(item);
}

void InventoryBox::AddItem(string name, int count)
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

	int maxCnt = data.maxCount;

	while (count != 0)
	{
		auto findPos = FindInvenPos(data.width, data.height);

		if (findPos == Vector2i{ -1, -1 })
		{
			return;
		}


		InvenItem* item = new InvenItem(uimgr);
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
			{ findPos.y, findPos.x }, data.path, data.maxCount, data.price);
		item->AddCount(count >= maxCnt ? maxCnt : count);
		item->SetName(name);
		item->Init();

		count -= maxCnt;
		count = max(count, 0);
		items.push_back(item);
	}
	inven->SetDrag(nullptr);
}
void InventoryBox::AddItem(string name, int count, Vector2i findPos)
{
	auto data = FILE_MGR->GetItemInfo(name);
	
	InvenItem* item = new InvenItem(uimgr);
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
		{ findPos.y, findPos.x }, data.path, data.maxCount, data.price);
	item->AddCount(count);
	item->SetName(name);
	item->Init();

	items.push_back(item);
	inven->SetDrag(nullptr);
}

Vector2i InventoryBox::FindInvenPos(int i_width, int i_height)
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

InvenGreed* InventoryBox::GetGreed(int i, int j)
{
	if (i >= height || j >= width)
		return nullptr;

	return itemGreed[i][j];
}

void InventoryBox::MoveItem(int i, int j)
{
	nowDrag->SetInvenPos({ startPos.x + i * 60 + padding * i , startPos.y + j * 60 + padding * j },
		{ i, j });

	int w = nowDrag->GetWidth();
	int h = nowDrag->GetHeight();
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			allPos[y + j][x + i] = true;
			itemGreed[y + j][x + i]->SetState(true, nowDrag);
		}
	}

	//if (find(items.begin(), items.end(), nowDrag) == items.end())
	//{
	//	//cout << "this11" << endl;
	//	//items.push_back(nowDrag);
	//}

	inven->MoveInvenItem(this);

	inven->SetDrag(nullptr);
	inven->SetPrevInven(nullptr);
}

void InventoryBox::MoveItem(InvenItem* item, Vector2i move_pos)
{
	auto data = FILE_MGR->GetItemInfo(item->GetName());

	for (int i = move_pos.y; i < move_pos.y + data.height; i++)
	{
		for (int j = move_pos.x; j < move_pos.x + data.width; j++)
		{
			allPos[i][j] = true;
			itemGreed[i][j]->SetState(true, item);

		}
	}
	item->Set(data.width, data.height,
		{ startPos.x + move_pos.x * 60 + padding * move_pos.x , startPos.y + move_pos.y * 60 + padding * move_pos.y },
		{ move_pos.x, move_pos.y }, data.path, data.maxCount, data.price);
	item->SetCount(item->GetCount());
	item->Init();

	items.push_back(item);
	inven->SetDrag(nullptr);
}

void InventoryBox::ReturnItem()
{
	int i = nowDrag->GetGreedPos().x;
	int j = nowDrag->GetGreedPos().y;

	if (i == -1 || j == -1)
	{
		inven->ReturnUseItem();
		return;
	}

	nowDrag->SetInvenPos({ startPos.x + i * 60 + padding * i , startPos.y + j * 60 + padding * j },
		{ i, j });

	int w = nowDrag->GetWidth();
	int h = nowDrag->GetHeight();
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			itemGreed[y + j][x + i]->SetState(true, nowDrag);
			allPos[y + j][x + i] = true;
		}
	}

	inven->SetDrag(nullptr);
	inven->SetPrevInven(nullptr);
}

void InventoryBox::DeleteItem(InvenItem* item)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (itemGreed[j][i]->GetItem() == item)
			{
				int w = item->GetWidth();
				int h = item->GetHeight();
				for (int x = 0; x < w; x++)
				{
					for (int y = 0; y < h; y++)
					{
						itemGreed[y + j][x + i]->SetState(false, nullptr);
						allPos[y + j][x + i] = false;
					}
				}
				items.erase(find(items.begin(), items.end(), item));
				return;
			}
		}
	}
}

void InventoryBox::DeleteItem(InvenItem* item,Vector2i startPos, Vector2i size)
{
	for (int i = startPos.y; i < startPos.y + size.y; i++)
	{
		for (int j = startPos.x; j < startPos.x + size.x; j++)
		{
			allPos[i][j] = false;
			itemGreed[i][j]->SetState(false, nullptr);
		}
	}

	auto it = find(items.begin(), items.end(), item);
	if(it != items.end())
		items.erase(it);
}

void InventoryBox::ClearInven()
{
	for (auto& item :items)
	{
		delete item;
	}
	items.clear();

	for (auto& greed_line : itemGreed)
	{
		for (auto& greed : greed_line)
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

void InventoryBox::Reset()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			itemGreed[i][j]->SetState(false, nullptr);
			allPos[i][j] = false;
		}
	}
}

