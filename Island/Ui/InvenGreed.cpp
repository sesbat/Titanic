#include "InvenGreed.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "Inventory.h"
#include "InvenItem.h"

InvenGreed::InvenGreed(UiMgr* mgr, Inventory* inven)
	:Button(mgr), inven(inven)
{
}

InvenGreed::~InvenGreed()
{
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
				greed->SetState(true);
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
				
				greed->SetState(inven->IsGreedItem(invenPos.y + j, invenPos.x + i));
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
				auto greed = inven->GetGreed(invenPos.y + j, invenPos.x + i);
				if (greed == nullptr || inven->IsGreedItem(invenPos.y + j, invenPos.x + i))
				{
					isMove = false;
					break;
					continue;
				}
			}
		}

		if (isMove)
			inven->MoveItem(invenPos.x, invenPos.y);
		else
			inven->ReturnItem();
	}
	Button::Update(dt);
}

void InvenGreed::Draw(RenderWindow& window)
{
	Button::Draw(window);
}

void InvenGreed::SetState(bool s)
{
	state = s;

	GetSpriteObj()->SetColor(!s ? Color::White : Color(0, 0, 0, 0));
}
