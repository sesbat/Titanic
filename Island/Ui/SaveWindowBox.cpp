#include "SaveWindowBox.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"

SaveWindowBox::SaveWindowBox(UiMgr* mgr)
	:Button(mgr)
{
}

SaveWindowBox::~SaveWindowBox()
{
}

void SaveWindowBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/window.png"),true);
	SetClkColor(false);
	
	save->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	save->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "SAVE", false);
	save->SetClkColor(true);
	save->SetPos(position + Vector2f{ 150, 255 });

	cancle->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	cancle->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "SAVE", false);
	cancle->SetClkColor(true);
	cancle->SetPos(position - Vector2f{ sprite->GetGlobalBound().width - 150, 255 });

	SetActive(false);
}

void SaveWindowBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	save->Draw(window);
	cancle->Draw(window);
}

void SaveWindowBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	save->Update(dt);
	cancle->Update(dt);

	{


	}
}

bool SaveWindowBox::IsSave()
{
	return save->IsDown() || save->IsClick();
}

bool SaveWindowBox::IsCancle()
{
	return cancle->IsDown() || cancle->IsClick();
}
