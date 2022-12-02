#include "MenuUiMgr.h"
#include "../../Scens/Scene.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include "../../Framework/ResourceManager.h"
#include "../../Framework/info.h"
#include "../../Scens/SceneManager.h"
#include "../../Ui/Button.h"
#include "../../Framework/InputMgr.h"
#include "../MoveObject.h"
#include "../DblClkObject.h"
#include "../Fade.h"

MenuUiMgr::MenuUiMgr(Scene* scene)
	:UiMgr(scene)
{
}
void MenuUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	{
		/*if (startBtn->IsDoubleClick())
		{
			cout << "dk" << endl;
		}*/
		/*if (startBtn->IsDoubleClickRight())
		{
			cout << "dkR" << endl;
		}*/

		if (startBtn1->IsUp())
		{
			SCENE_MGR->ChangeScene(Scenes::Ready);
			return;
		}
		if (startBtn2->IsUp())
		{
			SCENE_MGR->ChangeScene(Scenes::MapEditor);
			return;
		}
		if (startBtn3->IsUp())
		{
			exit(1);
		}
		if (InputMgr::GetKeyDown(Keyboard::Enter))
		{
			fade->SetSkip();
		}
	}
	fade->Update(dt);
}

void MenuUiMgr::Init()
{
	/*startBtn = new DblClkObject(this);
	startBtn->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn->SetPos(Vector2f{ WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 50 });
	startBtn->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn);*/


	startBtn1 = new Button(this);
	startBtn1->SetClkColor(true);
	/*startBtn2->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White,"Start",true);*/
	startBtn1->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "Start", true);
	startBtn1->SetOrigin(Origins::MC);
	startBtn1->SetPos({ WINDOW_WIDTH -200,WINDOW_HEIGHT / 2 + 100 });
	uiObjList[0].push_back(startBtn1);

	startBtn2 = new Button(this);
	startBtn2->SetClkColor(true);
	startBtn2->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "Settings", true);
	startBtn2->SetOrigin(Origins::MC);
	startBtn2->SetPos({ WINDOW_WIDTH -200,WINDOW_HEIGHT / 2 + 200 });
	uiObjList[0].push_back(startBtn2);

	startBtn3 = new Button(this);
	startBtn3->SetClkColor(true);
	startBtn3->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "Exit", true);
	startBtn3->SetOrigin(Origins::MC);
	startBtn3->SetPos({ WINDOW_WIDTH - 200,WINDOW_HEIGHT / 2 + 300 });
	uiObjList[0].push_back(startBtn3);


	//startBtn4 = new Button(this);
	//startBtn4->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	//startBtn4->SetPos(Vector2f{ WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 - 50 });
	//startBtn4->SetOrigin(Origins::MC);
	//uiObjList[0].push_back(startBtn4);

	//startBtn5 = new Button(this);
	//startBtn5->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	//startBtn5->SetPos(Vector2f{ WINDOW_WIDTH / 2 , WINDOW_HEIGHT / 2 });
	//startBtn5->SetOrigin(Origins::MC);
	//uiObjList[0].push_back(startBtn5);

	/*move = new MoveObject(this);
	move->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	move->SetPos(Vector2f{ 0 , 0 });
	move->SetOrigin(Origins::MC);
	uiObjList[0].push_back(move);*/
	fade = new Fade();
	fade->SetRect({ WINDOW_WIDTH,WINDOW_HEIGHT });
	fade->SetFadeIn();
	uiObjList[0].push_back(fade);
}

void MenuUiMgr::Release()
{
	UiMgr::Release();
}

void MenuUiMgr::Reset()
{
}

void MenuUiMgr::ClickStartBtn()
{
	SCENE_MGR->ChangeScene(Scenes::Ready);
}
