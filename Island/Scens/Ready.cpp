#include "Ready.h"
#include "SceneManager.h"
#include "../../Framework/InputMgr.h"
#include "../../Framework/ResourceManager.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include "../../Framework/Utils.h"
#include "../../Framework/info.h"
#include "../../Ui/Ready/ReadyUiMgr.h"
#include "../../Framework/SoundManager.h"

Ready::Ready()
	:Scene(Scenes::Ready)
{

}

Ready::~Ready()
{
	Release();
}

void Ready::Init()
{
	backGround = new SpriteObject();
	backGround->SetTexture(*RESOURCES_MGR->GetTexture("graphics/Menu/back.png"));
	backGround->SetSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
	backGround->SetPos({ 0,0 });

	objList[LayerType::Back][0].push_back(backGround);


	uiMgr = new ReadyUiMgr(this);
	uiMgr->Init();
}

void Ready::Release()
{
}

void Ready::Enter()
{
	Init();
	if (SOUND_MGR->GetNowBgm() != "sounds/menu.wav")
	{
		SOUND_MGR->StopAll();
		SOUND_MGR->Play("sounds/menu.wav", true);
		SOUND_MGR->SetNowBgm("sounds/menu.wav");
	}
	SCENE_MGR->GetCurrScene()->SetViewStop();
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
}

void Ready::Exit()
{
	Release();
}

void Ready::Update(float dt)
{
	Scene::Update(dt);
	if (InputMgr::GetKeyDown(Keyboard::Tab))
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
		return;
	}
}

void Ready::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void Ready::Reset()
{
	uiMgr->Init();
}
