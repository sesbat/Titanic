#include "BoolWindowBox.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/TextObject.h"
#include "../Framework/Framework.h"

BoolWindowBox::BoolWindowBox(UiMgr* mgr)
	:Button(mgr)
{
}

BoolWindowBox::~BoolWindowBox()
{
	Release();
}

void BoolWindowBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/window.png"), true);
	SetOrigin(Origins::MC);
	SetClkColor(false);

	yes = new Button(uimgr);
	yes->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	yes->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		45, Color::White, "YES", false);
	yes->SetClkColor(true);
	yes->SetPos({(float)FRAMEWORK->GetWindowSize().x/2 - 200,(float)FRAMEWORK->GetWindowSize().y/2 + 100});
	yes->SetOrigin(Origins::MC);
	yes->GetTextObj()->SetPos(yes->GetPos() + Vector2f{ 0, -15 });

	no = new Button(uimgr);
	no->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/button.png"), true);
	no->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		45, Color::White, "NO", false);
	no->SetClkColor(true);
	no->SetPos({ (float)FRAMEWORK->GetWindowSize().x/2 + 200,(float)FRAMEWORK->GetWindowSize().y/2 + 100 });
	no->SetOrigin(Origins::MC);
	no->GetTextObj()->SetPos(no->GetPos() + Vector2f{ 0, -15 });

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
		38, Color::White, "cost500 OK?");
	txt->SetPos({ (float)FRAMEWORK->GetWindowSize().x/2,(float)FRAMEWORK->GetWindowSize().y/2 });
	txt->SetOrigin(Origins::MC);
	SetActive(true);


	txt2 = new TextObject();
	txt2->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		38, Color::White, "");
	txt2->SetPos(position + Vector2f{ 250, 100 });
	txt2->SetOrigin(Origins::MC);
	SetActive(false);
}

void BoolWindowBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	yes->Draw(window);
	no->Draw(window);
	//cancle->Draw(window);
	txt->Draw(window);
	txt2->Draw(window);
}

void BoolWindowBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);

	if (yes->IsClick())
		isClickYes = true;
	else
		isClickYes = false;

	if (no->IsClick())
		isClickNo = true;
	else
		isClickNo = false;

	yes->Update(dt);
	no->Update(dt);
	//cancle->Update(dt);

	
}
void BoolWindowBox::Release()
{
	if (yes != nullptr)
	{
		delete yes;
		yes = nullptr;
	}
	if (txt != nullptr)
	{
		delete txt;
		txt = nullptr;
	}
	if (no != nullptr)
	{
		delete no;
		no = nullptr;
	}
	if (txt2 != nullptr)
	{
		delete txt2;
		txt2 = nullptr;
	}
	path = "";
}
void BoolWindowBox::SetPath(string path)
{
	this->path = path;
	txt->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		38, Color::White, path + "");
}

//bool SaveWindowBox::IsCancle()
//{
//	return cancle->IsDown() || cancle->IsClick();
//}
