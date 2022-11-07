#include "MapEditor.h"
#include "../Ui/Button.h"
#include "../Framework/ResourceManager.h"
#include "../Scens/SceneManager.h"
#include "../Framework/info.h"
#include "../../Ui/EditorMapUiMgr.h"
#include "../../Ui/DrawObj.h"
#include "../../Framework/InputMgr.h"
#include <algorithm>

MapEditor::MapEditor()
	: Scene(Scenes::MapEditor)
{
}

void MapEditor::Init()
{
	Reset();
}

void MapEditor::Reset()
{
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			auto tile = new Button(nullptr);
			greeds[i].push_back(tile);
			tile->SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/greed.png"), true);
			tile->SetPos({ 60.f * j, 60.f * i });
			objList[LayerType::Plat][i].push_back(tile);
			tile->SetUiView(false);
		}
	}
	uiMgr = new EditorMapUiMgr(this);
	uiMgr->Init();
}

void MapEditor::Update(float dt)
{
	Scene::Update(dt);
	
	if (InputMgr::GetKeyDown(Keyboard::E))
	{
		((EditorMapUiMgr*)uiMgr)->DeleteDraw();

	}
	if (InputMgr::GetMouseButtonDown(Mouse::Right))
	{
		initMousePos = InputMgr::GetMousePos();
		isMove = true;
	}
	if (isMove)
	{
		auto deltaPos = InputMgr::GetMousePos() - initMousePos;
		initMousePos = InputMgr::GetMousePos();
		cout << deltaPos.x << endl;
		cout << deltaPos.y << endl;
		auto movePos = SCENE_MGR->GetCurrScene()->GetWorldView().getCenter() - deltaPos;
		SCENE_MGR->GetCurrScene()->GetWorldView().setCenter(movePos);
	}
	if (InputMgr::GetMouseButtonUp(Mouse::Right))
	{
		isMove = false;
	}

	
	if (InputMgr::GetMouseWheelUp())
	{
		SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() - (Vector2f{19.2,10.8} * 3.f));
	}
	if (InputMgr::GetMouseWheelDown())
	{
		SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() + (Vector2f{ 19.2,10.8 } *3.f));
	}
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			if (greeds[i][j]->IsClick())
			{
				cout << i << endl;
				cout << j << endl;
				cout << "clickTile" << endl;
				DrawObj* nowDraw = ((EditorMapUiMgr*)uiMgr)->GetDraw();
				if (nowDraw == nullptr || ((EditorMapUiMgr*)uiMgr)->IsUnder())
				{
					Button* findObj = nullptr;
					if (greedObjs.find(i) != greedObjs.end())
					{
						if (greedObjs[i].find(j) != greedObjs[i].end())
						{
							findObj = greedObjs[i][j];
							auto deleteObj = find(objList[LayerType::Object][i].begin(), objList[LayerType::Object][i].end(), findObj);
							objList[LayerType::Object][i].erase(deleteObj);
							greedObjs[i].erase(greedObjs[i].find(j));

							delete findObj;
						}
					}
					return;
				}

				Button* findObj = nullptr;

				if (greedObjs.find(i) != greedObjs.end())
				{
					if (greedObjs[i].find(j) != greedObjs[i].end())
					{
						findObj = greedObjs[i][j];
						auto deleteObj = find(objList[LayerType::Object][i].begin(), objList[LayerType::Object][i].end(), findObj);
						objList[LayerType::Object][i].erase(deleteObj);
						greedObjs[i].erase(greedObjs[i].find(j));

						delete findObj;
					}
				}


				DrawObj* draw = new DrawObj(uiMgr);
				draw->SetType(nowDraw->GetType());
				draw->SetPath(nowDraw->GetPath());
				draw->SetTexture(*RESOURCES_MGR->GetTexture(draw->GetPath()),true);
				draw->SetOrigin(Origins::BC);
				draw->SetMove(false);
				draw->SetPos(greeds[i][j]->GetPos() + Vector2f{30.f, 60.f});
				objList[LayerType::Object][i].push_back(draw);
				greedObjs[i][j] = draw;
			}
		}
	}
}

void MapEditor::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}


void MapEditor::Enter()
{
	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	Init();
}

void MapEditor::Exit()
{
}

MapEditor::~MapEditor()
{
}