#include "InvenItem.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../GameObject/ToolTip.h"
#include "../Scens/SceneManager.h"
#include "../Scens/Ready.h"
#include "../Ui/Ready/ReadyUiMgr.h"
#include "../Ui/GameSceneUiMgr.h"


InvenItem::InvenItem(UiMgr* mgr)
	:MoveObject(mgr)
{
	invenPos = { 0,0 };
}

InvenItem::~InvenItem()
{
	Release();
}

void InvenItem::Release()
{
	MoveObject::Release();
}

void InvenItem::Init()
{
	SetMove(true);
	SetClkColor(true);

}

void InvenItem::Update(float dt)
{
	if (!enabled)
		return;
	MoveObject::Update(dt);

	if (IsDrag())
	{
		GetTextObj()->SetPos(position + Vector2f(bound.width, bound.height));
	}
	ToolTipUpdate(dt);
}
void InvenItem::ButtonUpdate(float dt)
{
	if (!enabled)
		return;

	Button::Update(dt);
	drag = false;
}

void InvenItem::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	MoveObject::Draw(window);
}

void InvenItem::Set(int width, int height, Vector2i invenPos,Vector2i invenGreedPos, string path, int maxCount, int price)
{
	this->width = width;
	this->height = height;
	this->invenPos = invenPos;
	this->invenGreedPos = invenGreedPos;
	this->path = path;
	this->maxCount = maxCount;
	this->price = price;
	
	SetPos((Vector2f)invenPos);
	SetTexture(*RESOURCES_MGR->GetTexture(path), true);
	GetTextObj()->SetPos(position + Vector2f(60, 60));

	auto data = FILE_MGR->GetItemInfo(name);
	SetOverlapTimer(data.tooltipDelay);

}

void InvenItem::SetInvenPos(Vector2i invenPos, Vector2i invenGreedPos)
{
	this->invenPos = invenPos;
	this->invenGreedPos = invenGreedPos;

	SetPos((Vector2f)invenPos);
	GetTextObj()->SetPos(position + Vector2f(60, 60));
}

void InvenItem::SetInvenPos(Vector2f invenGreedPos)
{
	this->invenPos = (Vector2i)invenGreedPos;

	SetPos((Vector2f)invenPos);
	GetTextObj()->SetPos(position + Vector2f(60, 60));
	this->invenGreedPos = { -1,-1 };
}

void InvenItem::AddCount(int n)
{
	count += n;
	if (count <= 0)
	{
		count = 0;		
		return;
	}
	if (maxCount != 1)
		SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			20, Color::White, to_string(count), false);
	GetTextObj()->SetOrigin(Origins::BR);
	GetTextObj()->SetPos(position + Vector2f(60, 60));

}

void InvenItem::SetCount(int n)
{
	count = n;
	if (count <= 0)
	{
		count = 0;
		return;
	}
	if (maxCount != 1)
		SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			20, Color::White, to_string(count), false);
	GetTextObj()->SetOrigin(Origins::BR);
	GetTextObj()->SetPos(position + Vector2f(60, 60));

}

void InvenItem::ToolTipUpdate(float dt)
{

	if (IsStay())
	{
		ToolTip* tip;
		tip = SCENE_MGR->GetCurrSceneType() == Scenes::Ready ? ((ReadyUiMgr*)uimgr)->GetTip() : ((GameSceneUiMgr*)uimgr)->GetTip();

		if (!tip->GetActive())
		{
			overLapTimer += dt;
			if (overLapTimer > initTimer)
			{
				tip->SetItem(GetName());
				tip->SetItemType(FILE_MGR->GetItemInfo(GetName()).type);
				tip->SetToolPos(InputMgr::GetMousePos());
				tip->init();
				tip->SetActive(true);
				overLapTimer = 0.f;
			}
		}
	}
}