#include "MapEditor.h"
#include "../Ui/Button.h"
#include "../Framework/ResourceManager.h"
#include "../Scens/SceneManager.h"
#include "../Framework/info.h"

MapEditor::MapEditor()
	: Scene(Scenes::MapEditor)
{
}

void MapEditor::Init()
{
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	Reset();
}

void MapEditor::Reset()
{
	for (int i = 0; i < WIDTHCNT; i++)
	{
		for (int j = 0; j < HEIGHTCNT; j++)
		{
			auto tile = new Button(nullptr);
			greeds[i].push_back(tile);
			tile->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/greed.png"), true);
			tile->SetPos({ 60.f * j, 60.f * i });
			objList[LayerType::Plat][i].push_back(tile);
		}
	}
}

void MapEditor::Update(float dt)
{
	Scene::Update(dt);
}

void MapEditor::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}


void MapEditor::Enter()
{
	Init();
}

void MapEditor::Exit()
{
}

MapEditor::~MapEditor()
{
}