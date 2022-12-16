#include "Tutorial.h"
#include "../GameObject/SpriteObject.h"
#include "../Ui/Button.h"
#include "../Framework/InputMgr.h"
#include "../Framework/FileManager.h"
#include "../Framework/ResourceManager.h"
#include "../Ui/TutorialUiMgr.h"
#include "../Framework/Framework.h"
#include "../Scens/Scene.h"
#include "../../Framework/info.h"
#include "../Scens/SceneManager.h"

Tutorial::Tutorial()
	:Scene(Scenes::Tutorial)
{
}

Tutorial::~Tutorial()
{
}

void Tutorial::Enter()
{
	Init();
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
}

void Tutorial::Exit()
{
}

void Tutorial::Init()
{
	auto data = FILE_MGR->GetTutorialPath();

	for (auto path : data)
	{
		SpriteObject* spr = new SpriteObject();
		spr->SetTexture(*RESOURCES_MGR->GetTexture(path));
		spr->SetPos(Vector2f{ FRAMEWORK->GetWindowSize().x / 2.f,FRAMEWORK->GetWindowSize().y / 2.f });
		spr->SetOrigin(Origins::MC);
		

		png.push_back(spr);
	}
	idx = 0;

	uiMgr = new TutorialUiMgr(this);
	((TutorialUiMgr*)uiMgr)->Init();
}

void Tutorial::Reset()
{
}

void Tutorial::Release()
{
}

void Tutorial::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		idx = (idx + png.size() + 1) % png.size();
	}
	uiMgr->Update(dt);
}

void Tutorial::Draw(RenderWindow& window)
{
	uiMgr->Draw(window);
	png[idx]->Draw(window);
}
