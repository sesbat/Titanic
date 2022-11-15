#include "Inventory.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "InvenItem.h"

Inventory::Inventory(UiMgr* mgr)
	: Button(mgr), totalWeight(0.f), nowDrag(nullptr)
{
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/inventory.png"), true);
	for (int i = 0; i < height; i++)
	{
		vector<bool> line;
		vector<InvenGreed*> greedLine;
		for (int j = 0; j < width; j++)
		{
			line.push_back(false);

			InvenGreed* greed = new InvenGreed(uimgr, this);
			greed->Init();
			greed->SetPos({ startPos1.x + j * 60.f + padding * j , startPos1.y + i * 60.f + padding * i });
			greed->SetInvenPos(j, i);
			greedLine.push_back(greed);
		}
		itemPos.push_back(line);
		itemGreed.push_back(greedLine);
	}

}

void Inventory::Update(float dt)
{
	if (!enabled)
		return;

	if (nowDrag != nullptr)
	{
		nowDrag->Update(dt);
	}
	else
	{
		for (auto& item : items)
		{
			item->Update(dt);
			if (item->IsDown())
			{
				nowDrag = item;
				auto invenPos = nowDrag->GetGreedPos();

				for (int i = invenPos.x; i < invenPos.x + nowDrag->GetWidth(); i++)
				{
					for (int j = invenPos.y; j < invenPos.y + nowDrag->GetHeight(); j++)
					{
						itemPos[j][i] = false;
						itemGreed[j][i]->SetState(false);
					}
				}
				//items.erase(items.begin(), find(items.begin(), items.end(), item));
				//items.push_back(item);
				break;
			}
		}
	}

	if (InputMgr::GetKeyDown(Keyboard::Q))
	{
		AddItem("Recoverykit");
	}
	if (InputMgr::GetKeyDown(Keyboard::W))
	{
		AddItem("handsaw");
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

void Inventory::Draw(RenderWindow& window)
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
	if(nowDrag != nullptr)
		nowDrag->Draw(window);
}

void Inventory::AddItem(string name)
{
	auto data = FILE_MGR->GetItemInfo(name);

	for (auto& item : items)
	{
		if (item->GetName() == name)
		{
			if (item->GetCount() < data.maxCount)
			{
				item->AddCount(1);
				totalWeight += data.weight;
				return;
			}
		}
	}

	auto findPos = FindInvenPos(data.weight, data.height);

	if (findPos == Vector2i{-1, -1})
	{
		return;
	}

	InvenItem* item = new InvenItem(uimgr);
	cout << "FindPos_X:" << findPos.x << endl;
	cout << "FindPos_Y:" << findPos.y << endl;
	for (int i = findPos.x; i < findPos.x + data.height; i++)
	{
		for (int j = findPos.y; j < findPos.y + data.weight; j++)
		{
			itemPos[i][j] = true;
			itemGreed[i][j]->SetState(true);
			cout << "unRock_i:" << i << endl;
			cout << "unRock_j:" << j << endl;
		}
	}
	item->Set(data.width, data.height,
		{ startPos1.x + findPos.y * 60 + padding * findPos.y , startPos1.y + findPos.x * 60 + padding * findPos.x },
		{ findPos.y, findPos.x }, data.path, data.maxCount);
	item->AddCount(1);
	item->SetName(name);
	item->Init();

	items.push_back(item);
}

Vector2i Inventory::FindInvenPos(int i_width, int i_height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width  ; j++)
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

InvenGreed* Inventory::GetGreed(int i, int j)
{
	if (i >= height || j >= width)
		return nullptr;

	return itemGreed[i][j];
}

void Inventory::MoveItem(int i, int j)
{
	nowDrag->SetInvenPos({ startPos1.x + i * 60 + padding * i , startPos1.y + j * 60 + padding * j },
		{ i, j });

	int w = nowDrag->GetWidth();
	int h = nowDrag->GetHeight();
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			itemPos[y + j][x + i]=true;
		}
	}
	nowDrag = nullptr;
}

void Inventory::ReturnItem()
{
	int i = nowDrag->GetGreedPos().x;
	int j = nowDrag->GetGreedPos().y;
	nowDrag->SetInvenPos({ startPos1.x + i * 60 + padding * i , startPos1.y + j * 60 + padding * j },
		{ i, j });

	int w = nowDrag->GetWidth();
	int h = nowDrag->GetHeight();
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			itemGreed[y+j][x+i]->SetState(true);
		}
	}
}
