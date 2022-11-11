#include "GameSceneUiMgr.h"
#include "Button.h"
#include "../Framework/ResourceManager.h"
#include "../Framework//info.h"
#include "../Framework/InputMgr.h"
#include "../../Scens/SceneManager.h"
#include "../../Framework/FileManager.h"
#include "../../Scens/GameScene.h"

GameSceneUiMgr::GameSceneUiMgr(Scene* scene)
	:UiMgr(scene)
{
}

GameSceneUiMgr::~GameSceneUiMgr()
{
}

void GameSceneUiMgr::Init()
{
	exitBtn = new Button(this);
	exitBtn->SetClkColor(true);
	exitBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "EXIT", true);
	exitBtn->SetOrigin(Origins::TL);
	exitBtn->SetPos({ 50,50 });
	uiObjList[0].push_back(exitBtn);
}

void GameSceneUiMgr::Reset()
{
}

void GameSceneUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
}

void GameSceneUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
}

bool GameSceneUiMgr::IsExit()
{
	return exitBtn->IsDown() || exitBtn->IsClick();
}
