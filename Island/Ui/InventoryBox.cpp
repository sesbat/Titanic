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
		itemPos.push_back(line);
		itemGreed.push_back(greedLine);
	}
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
				cout << GetName() << endl;
				inven->SetDrag(item);
				inven->SetPrevInven(this);
				auto invenPos = nowDrag->GetGreedPos();

				for (int i = invenPos.x; i < invenPos.x + nowDrag->GetWidth(); i++)
				{
					for (int j = invenPos.y; j < invenPos.y + nowDrag->GetHeight(); j++)
					{
						itemPos[j][i] = false;
						itemGreed[j][i]->SetState(false, nullptr);
					}
				}
				//items.erase(items.begin(), find(items.begin(), items.end(), item));
				//items.push_back(item);
				break;
			}
			else if (item->IsClickRight())
			{
				auto pair = inven->GetPairBox(inven->GetNowInven());

				cout << pair->GetName() << endl;
				auto pos = pair->FindInvenPos(item->GetWidth(), item->GetHeight());

				cout << pos.x << endl;
				cout << pos.y << endl;
				auto invenPos = item->GetGreedPos();


				if (pos != Vector2i{ -1,-1 })
				{
					for (int i = invenPos.x; i < invenPos.x + item->GetWidth(); i++)
					{
						for (int j = invenPos.y; j < invenPos.y + item->GetHeight(); j++)
						{
							itemPos[j][i] = false;
							itemGreed[j][i]->SetState(false, nullptr);
						}
					}

					pair->AddItem(item->GetName(), item->GetCount());
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

	auto findPos = FindInvenPos(data.weight, data.height);

	if (findPos == Vector2i{ -1, -1 })
	{
		return;
	}

	int maxCnt = data.maxCount;

	InvenItem* item = new InvenItem(uimgr);
	for (int i = findPos.x; i < findPos.x + data.height; i++)
	{
		for (int j = findPos.y; j < findPos.y + data.weight; j++)
		{
			itemPos[i][j] = true;
			itemGreed[i][j]->SetState(true, item);
			cout << "unRock_i:" << i << endl;
			cout << "unRock_j:" << j << endl;
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

Vector2i InventoryBox::FindInvenPos(int i_width, int i_height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (itemPos[i][j])
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
					if (itemPos[n_i][n_j] || j + i_width > width)
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
			itemPos[y + j][x + i] = true;
			itemGreed[y + j][x + i]->SetState(true, nowDrag);
		}
	}

	inven->MoveInvenItem(this);
	inven->SetDrag(nullptr);
	inven->SetPrevInven(nullptr);
}

void InventoryBox::ReturnItem()
{
	int i = nowDrag->GetGreedPos().x;
	int j = nowDrag->GetGreedPos().y;
	nowDrag->SetInvenPos({ startPos.x + i * 60 + padding * i , startPos.y + j * 60 + padding * j },
		{ i, j });

	int w = nowDrag->GetWidth();
	int h = nowDrag->GetHeight();
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			itemGreed[y + j][x + i]->SetState(true, nowDrag);
			itemPos[y + j][x + i] = true;
		}
	}

	inven->SetDrag(nullptr);
	inven->SetPrevInven(nullptr);
}

