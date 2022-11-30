#include "SaveWindowBox.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/TextObject.h"

SaveWindowBox::SaveWindowBox(UiMgr* mgr)
	:Button(mgr)
{
}

SaveWindowBox::~SaveWindowBox()
{
	if (save != nullptr)
	{
		delete save;
		save = nullptr;
	}
	//if (cancle != nullptr)
	//{
	//	delete cancle;
	//	cancle = nullptr;
	//}
	if (txt != nullptr)
	{
		delete txt;
		txt = nullptr;
	}
}

void SaveWindowBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/window.png"), true);
	SetClkColor(false);

	save = new Button(uimgr);
	save->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	save->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		45, Color::White, "SAVE", false);
	save->SetClkColor(true);
	save->SetPos(position + Vector2f{ 150, 255 });
	save->SetOrigin(Origins::MC);
	save->GetTextObj()->SetPos(save->GetPos() + Vector2f{ 0, -15 });

	//cancle = new Button(uimgr);
	//cancle->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	//cancle->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
	//	38, Color::White, "CANCLE", false);
	//cancle->SetClkColor(true);
	//cancle->SetPos(position + Vector2f{ sprite->GetGlobalBound().width - 150, 255 });
	//cancle->SetOrigin(Origins::MC);
	//cancle->GetTextObj()->SetPos(cancle->GetPos() + Vector2f{0, -15});

	txt = new TextObject();
	txt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		38, Color::White, "");
	txt->SetPos(position + Vector2f{ 150, 100 });
	txt->SetOrigin(Origins::TL);
	SetActive(false);
}

void SaveWindowBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	save->Draw(window);
	//cancle->Draw(window);
	txt->Draw(window);
}

void SaveWindowBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	save->Update(dt);
	//cancle->Update(dt);

	if (InputMgr::GetKeyDown())
	{
		if (!InputMgr::GetKeyDown(Keyboard::BackSpace))
		{
			path += InputMgr::GetLastKey();
			txt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
				38, Color::White, path + "");
			txt->SetPos(position + Vector2f{ 150, 100 });
			txt->SetOrigin(Origins::TL);
		}
		else if (path != "")
		{
			path.erase(path.end() - 1);
			txt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
				38, Color::White, path + "");
		}
	}
}
void SaveWindowBox::Release()
{
	if (save != nullptr)
	{
		delete save;
		save = nullptr;
	}
	//if (cancle != nullptr)
	//{
	//	delete cancle;
	//	cancle = nullptr;
	//}
	if (txt != nullptr)
	{
		delete txt;
		txt = nullptr;
	}
	path = "";
}
void SaveWindowBox::SetPath(string path)
{
	this->path = path;
	txt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		38, Color::White, path + "");
}
bool SaveWindowBox::IsSave()
{
	return save->IsUp();
}

//bool SaveWindowBox::IsCancle()
//{
//	return cancle->IsDown() || cancle->IsClick();
//}
