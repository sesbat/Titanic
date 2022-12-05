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
	: Button(mgr), nowDrag(nullptr), inven(inven), startPos(startPos)
{
}

CraftBox::~CraftBox()
{
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
	craftButton = new Button(uimgr);
	craftButton->SetClkColor(true);
	craftButton->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		50, Color::White, "Craft", true);
	craftButton->SetOrigin(Origins::MC);
	craftButton->SetPos({ 1500.f,800.f});

	player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
}

void CraftBox::Update(float dt)
{
	if (!enabled)
		return;

	//if (nowDrag == nullptr)
	//{
	//	for (auto& item : items)
	//	{
	//		item->Update(dt);
	//		if (item->IsDown())
	//		{
	//			inven->SetDrag(item);
	//			inven->SetPrevInven(this);
	//			auto invenPos = nowDrag->GetGreedPos();

	//			for (int i = invenPos.x; i < invenPos.x + nowDrag->GetWidth(); i++)
	//			{
	//				for (int j = invenPos.y; j < invenPos.y + nowDrag->GetHeight(); j++)
	//				{
	//					allPos[j][i] = false;
	//					itemGreed[j][i]->SetState(false, nullptr);
	//				}
	//			}
	//			//items.erase(items.begin(), find(items.begin(), items.end(), item));
	//			//items.push_back(item);
	//			break;
	//		}
	//		else if (item->IsClickRight())
	//		{
	//			auto pos = pairInven->FindInvenPos(item->GetWidth(), item->GetHeight());
	//			auto invenPos = item->GetGreedPos();


	//			if (pos != Vector2i{ -1,-1 })
	//			{
	//				for (int i = invenPos.x; i < invenPos.x + item->GetWidth(); i++)
	//				{
	//					for (int j = invenPos.y; j < invenPos.y + item->GetHeight(); j++)
	//					{
	//						allPos[j][i] = false;
	//						itemGreed[j][i]->SetState(false, nullptr);
	//					}
	//				}

	//				pairInven->AddItem(item->GetName(), item->GetCount());
	//				items.erase(find(items.begin(), items.end(), item));
	//				break;
	//			}
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	nowDrag->Update(dt);
	//}
	craftButton->Update(dt);
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
					//cout << n->GetName() << endl;
					inven->OnClickCraftItem(FILE_MGR->GetCraftItemInfo(n->GetName()));
					craftingItmeName = n->GetName();
				}
			}
		}
	}

	if (!isRequired && craftButton->IsClick())
	{
		if (Crafting(craftingItmeName))
		{
			player->GetInventory()->GetPlayerInven()->AddItem(craftingItmeName);
			DeletePlayerItem(craftingItmeName);
		}
		else
		{
			//cout << "Not Making" << endl;
			//cout << craftingItmeName << endl;
			//cout << this << endl;
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
		if (item == nowDrag)
			continue;
		item->Draw(window);
	}
	craftButton->Draw(window);
	if (nowDrag != nullptr)
		nowDrag->Draw(window);
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

void CraftBox::MoveItem(int i, int j)
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

void CraftBox::ReturnItem()
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

