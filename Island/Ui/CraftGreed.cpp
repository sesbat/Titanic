#include "CraftGreed.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "Craft.h"
#include "InvenItem.h"
#include "CraftBox.h"
#include "CraftItem.h"

CraftGreed::CraftGreed(UiMgr* mgr, Craft* inven, CraftBox* invenbox)
	:Button(mgr), inven(inven), invenBox(invenbox), state(false)
{
}

CraftGreed::~CraftGreed()
{
	Release();
}

void CraftGreed::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/invenGreed.png"), true);
}

void CraftGreed::Update(float dt)
{

}

void CraftGreed::Draw(RenderWindow& window)
{
	Button::Draw(window);
}

void CraftGreed::SetState(bool s, CraftItem* item)
{
	state = s;

	nowItem = item;

	GetSpriteObj()->SetColor(!s ? Color::White : Color(0, 0, 0, 0));
}
