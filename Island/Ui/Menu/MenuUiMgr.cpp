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

MenuUiMgr::MenuUiMgr(Scene* scene)
	:UiMgr(scene)
{

}
void MenuUiMgr::Update(float dt)
{
	UiMgr::Update(dt);

	{
		if (startBtn->IsDoubleClick())
		{
			cout << "dk" << endl;
		}
		if (startBtn->IsDoubleClickRight())
		{
			cout << "dkR" << endl;
		}

		if (startBtn5->IsUp())
		{
			SCENE_MGR->ChangeScene(Scenes::Ready);
			return;
		}
	}
}

void MenuUiMgr::Init()
{
	startBtn = new DblClkObject(this);
	startBtn->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn->SetPos(Vector2f{ WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 50 });
	startBtn->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn);


	startBtn2 = new Button(this);
	startBtn2->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn2->SetPos(Vector2f{ WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 + 50 });
	startBtn2->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn2);

	startBtn3 = new Button(this);
	startBtn3->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn3->SetPos(Vector2f{ WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50 });
	startBtn3->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn3);

	startBtn4 = new Button(this);
	startBtn4->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn4->SetPos(Vector2f{ WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 - 50 });
	startBtn4->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn4);

	startBtn5 = new Button(this);
	startBtn5->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	startBtn5->SetPos(Vector2f{ WINDOW_WIDTH / 2 , WINDOW_HEIGHT / 2 });
	startBtn5->SetOrigin(Origins::MC);
	uiObjList[0].push_back(startBtn5);

	move = new MoveObject(this);
	move->SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	move->SetPos(Vector2f{ 0 , 0 });
	move->SetOrigin(Origins::MC);
	uiObjList[0].push_back(move);
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
