#include "invenPos.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"

InvenItem::InvenItem(UiMgr* mgr)
	:Button(mgr)
{
	countTxt = new TextObject();
	countTxt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, "0");
	invenPos = { 0,0 };
}

InvenItem::~InvenItem()
{
}

void InvenItem::Init()
{
}

void InvenItem::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
}

void InvenItem::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
}
