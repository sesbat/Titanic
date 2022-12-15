#include "CraftItem.h"
#include "InvenItem.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Scens/SceneManager.h"
#include "../Scens/Ready.h"
#include "../Ui/Ready/ReadyUiMgr.h"
#include "../GameObject/ToolTip.h"

CraftItem::CraftItem(UiMgr* mgr)
	:Button(mgr), material(3)
{
	invenPos = { 0,0 };
}

CraftItem::~CraftItem()
{
	Release();
}

void CraftItem::Init()
{
	SetMove(true);
	SetClkColor(true);
}

void CraftItem::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);

	if (IsDrag())
	{
		GetTextObj()->SetPos(position + Vector2f(bound.width, bound.height));
	}
	ToolTipUpdate(dt);
}

void CraftItem::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
}

void CraftItem::Set(int width, int height, Vector2i invenPos, Vector2i invenGreedPos, string path, int maxCount)
{
	this->width = width;
	this->height = height;
	this->invenPos = invenPos;
	this->invenGreedPos = invenGreedPos;
	this->path = path;
	this->maxCount = maxCount;


	SetPos((Vector2f)invenPos);
	SetTexture(*RESOURCES_MGR->GetTexture(path), true);
	GetTextObj()->SetPos(position + Vector2f(60, 60));

	auto data = FILE_MGR->GetItemInfo(name);
	SetOverlapTimer(data.tooltipDelay);
}

void CraftItem::SetInvenPos(Vector2i invenPos, Vector2i invenGreedPos)
{
	this->invenPos = invenPos;
	this->invenGreedPos = invenGreedPos;

	SetPos((Vector2f)invenPos);
	GetTextObj()->SetPos(position + Vector2f(60, 60));
}

void CraftItem::SetInvenPos(Vector2f invenGreedPos)
{
	this->invenPos = (Vector2i)invenGreedPos;

	SetPos((Vector2f)invenPos);
	GetTextObj()->SetPos(position + Vector2f(60, 60));
	this->invenGreedPos = { -1,-1 };
}

void CraftItem::AddCount(int n)
{
	count += n;

	if (maxCount != 1)
		SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			20, Color::White, to_string(count), false);
	GetTextObj()->SetOrigin(Origins::BR);
	GetTextObj()->SetPos(position + Vector2f(60, 60));
}

void CraftItem::ToolTipUpdate(float dt)
{
	if (IsStay())
	{
		ToolTip* tip;
		tip = ((ReadyUiMgr*)uimgr)->GetTip();

		if (!tip->GetActive())
		{
			overLapTimer += dt;
			//cout << overLapTimer << endl;
			if (overLapTimer > initTimer)
			{
				tip->SetItem(GetName());
				tip->SetItemType(FILE_MGR->GetItemInfo(GetName()).type);
				tip->SetToolPos(SCENE_MGR->GetCurrScene()->ScreenToWorld((Vector2i)InputMgr::GetMousePos()));
				tip->init();
				tip->SetActive(true);
				overLapTimer = 0.f;
			}
		}
	}
}

void CraftItem::SetActive(bool state)
{
	Button::SetActive(state);
	overLapTimer = 0.f;
}