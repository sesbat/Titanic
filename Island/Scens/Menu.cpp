#include "Menu.h"
#include "SceneManager.h"
#include "../../Framework/InputMgr.h"
#include "../../Framework/ResourceManager.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include "../../Framework/Utils.h"
#include "../../Framework/info.h"
#include "../../Ui/Menu/MenuUiMgr.h"
#include "../../Framework/Framework.h"
#include "../../Framework/SoundManager.h"

Menu::Menu()
	:Scene(Scenes::Menu)
{

}

Menu::~Menu()
{
	Release();
}

void Menu::Init()
{
	backGround = new SpriteObject();
	backGround->SetTexture(*RESOURCES_MGR->GetTexture("graphics/title.png"));
	backGround->SetSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
	backGround->SetPos({ 0,0 });

	objList[LayerType::Back][0].push_back(backGround);

	/*start = new TextObject();
	start->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "Game Start");
	start->SetOrigin(Origins::MC);
	start->SetPos({ WINDOW_WIDTH/2,WINDOW_HEIGHT /2+100});*/
	//objList[LayerType::Back][0].push_back(start);


	//cursor = new SpriteObject();
	//cursor->SetTexture(*RESOURCES_MGR->GetTexture("graphics/cursor.png"));
	//cursor->SetOrigin(Origins::MC);

	uiMgr = new MenuUiMgr(this);
	((MenuUiMgr*)uiMgr)->Init();
}

void Menu::Release()
{
	Scene::Release();
}

void Menu::Enter()
{
	Init();
	//if (SOUND_MGR->GetNowBgm() != "sounds/menu.wav")
	//{
	//	SOUND_MGR->StopAll();
	//	SOUND_MGR->Play("sounds/menu.wav", true);
	//	SOUND_MGR->SetNowBgm("sounds/menu.wav");
	//}
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
}

void Menu::Exit()
{
	Release();
}

void Menu::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		FRAMEWORK->GetWindow().close();
		return;
	}
	//cursor->SetPos(InputMgr::GetMousePos());
}

void Menu::Draw(RenderWindow& window)
{
	Scene::Draw(window);
	//cursor->Draw(window);
}

void Menu::Reset()
{
	uiMgr->Init();
}
