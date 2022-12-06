#include "InvenGreed.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "Inventory.h"
#include "InvenItem.h"
#include "InventoryBox.h"

InvenGreed::InvenGreed(UiMgr* mgr, Inventory* inven, InventoryBox* invenbox)
	:Button(mgr), inven(inven), invenBox(invenbox), state(false)
{
}

InvenGreed::~InvenGreed()
{
	Button::Release();
}

void InvenGreed::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/invenGreed.png"), true);
}

void InvenGreed::Update(float dt)
{
	InvenItem* dragItem = inven->GetNowDrag();
	if (IsEnter() && dragItem != nullptr)
	{
		int w = dragItem->GetWidth();
		int h = dragItem->GetHeight();

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				auto greed = inven->GetGreed(invenPos.y + j, invenPos.x + i);
				if (greed == nullptr)
					continue;
				greed->SetState(true, nowItem);
			}
		}
	}
	if (IsExit() && dragItem != nullptr)
	{
		int w = dragItem->GetWidth();
		int h = dragItem->GetHeight();

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				auto greed = inven->GetGreed(invenPos.y + j, invenPos.x + i);
				if (greed == nullptr)
					continue;

				greed->SetState(inven->GetNowInven()->IsGreedItem(invenPos.y + j, invenPos.x + i), greed->GetItem());
			}
		}
	}

	bool isMove = true;
	if (IsUp() && dragItem != nullptr)
	{
		int w = dragItem->GetWidth();
		int h = dragItem->GetHeight();

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				auto greed = inven->GetNowInven()->GetGreed(invenPos.y + j, invenPos.x + i);
				if (greed == nullptr || inven->GetNowInven()->IsGreedItem(invenPos.y + j, invenPos.x + i))
				{
					isMove = false;
					break;
					continue;
				}
			}
		}
		if (isMove)
		{
			inven->GetNowInven()->MoveItem(invenPos.x, invenPos.y);
			return;
		}
		else
		{
			bool isReturn = true;
			if (nowItem != nullptr)
				if (nowItem->GetName() == dragItem->GetName())
				{
					int nowCnt = nowItem->GetCount();
					int dragCnt = dragItem->GetCount();

					int maxCnt = nowItem->GetMaxCount();

					if (nowCnt + dragCnt <= maxCnt)
					{
						nowItem->AddCount(dragCnt);

						isReturn = false;

						if (inven->GetUseIdx() == -1)
						{
							auto items = inven->GetPrevInven()->GetItems();
							items->erase(find(items->begin(), items->end(), dragItem));
						}
						else
						{

						}

						delete dragItem;
						inven->SetDrag(nullptr);
					}
					else
					{
						int addCnt = maxCnt - nowCnt;
						nowItem->AddCount(addCnt);
						dragItem->AddCount(-addCnt);

						isReturn = true;

					}

				}
			if (isReturn)
			{
				for (int i = 0; i < w; i++)
				{
					for (int j = 0; j < h; j++)
					{
						auto greed = inven->GetGreed(invenPos.y + j, invenPos.x + i);
						if (greed != nullptr)
						{
							greed->SetState(invenBox->IsGreedItem(invenPos.y + j, invenPos.x + i), greed->GetItem());
						}
					}
				}
				inven->GetPrevInven()->ReturnItem();
				return;
			}
		}
	}
	Button::Update(dt);
}

void InvenGreed::Draw(RenderWindow& window)
{
	Button::Draw(window);
}

void InvenGreed::SetState(bool s, InvenItem* item)
{
	state = s;

	nowItem = item;

	GetSpriteObj()->SetColor(!s ? Color::White : Color(0, 0, 0, 0));
}
