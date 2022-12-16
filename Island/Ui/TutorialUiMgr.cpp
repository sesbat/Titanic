#include "TutorialUiMgr.h"
#include "Button.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Scens/SceneManager.h"
#include "../Framework/Framework.h"

TutorialUiMgr::TutorialUiMgr(Scene* scene)
	:UiMgr(scene)
{
}

void TutorialUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	
	if (exit->IsClick())
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
	}
}

void TutorialUiMgr::Draw(RenderWindow& window)
{
	UiMgr::Draw(window);
}

void TutorialUiMgr::Init()
{
	UiMgr::Init();
	exit = new Button(nullptr);
	exit->SetClkColor(true);
	exit->SetTexture(*RESOURCES_MGR->GetTexture("graphics/tt_exit.png"), true);
	exit->SetOrigin(Origins::TR);
	exit->SetPos(Vector2f{ (float)FRAMEWORK->GetWindowSize().x,0.f});

	uiObjList[0].push_back(exit);
}

void TutorialUiMgr::Release()
{
	UiMgr::Release();
}

void TutorialUiMgr::Reset()
{
	UiMgr::Reset();
}

