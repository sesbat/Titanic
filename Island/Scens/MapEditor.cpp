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
			objList[LayerType::Back][i].push_back(tile);
			tile->SetUiView(false);
		}
	}
	uiMgr = new EditorMapUiMgr(this);
	uiMgr->Init();
	nowType = LayerType::Object;
}

void MapEditor::Update(float dt)
{
	Scene::Update(dt);
	
	if (InputMgr::GetKeyDown(Keyboard::S))
	{
		Save();
	}
	if (InputMgr::GetKeyDown(Keyboard::L))
	{
		Load();
	}

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
				auto& nowGreedObjs = greedObjs[nowType];

				if (nowDraw == nullptr || ((EditorMapUiMgr*)uiMgr)->IsUnder())
				{
					Button* findObj = nullptr;
					if (nowGreedObjs.find(i) != nowGreedObjs.end())
					{
						if (nowGreedObjs[i].find(j) != nowGreedObjs[i].end())
						{
							findObj = nowGreedObjs[i][j];
							auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
							objList[nowType][i].erase(deleteObj);
							greedObjs[nowType][i].erase(nowGreedObjs[i].find(j));

							delete findObj;
						}
					}
					return;
				}

				Button* findObj = nullptr;
				if (nowGreedObjs.find(i) != nowGreedObjs.end())
				{
					if (nowGreedObjs[i].find(j) != nowGreedObjs[i].end())
					{
						findObj = nowGreedObjs[i][j];
						auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
						objList[nowType][i].erase(deleteObj);
						greedObjs[nowType][i].erase(nowGreedObjs[i].find(j));

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
				draw->SetData(nowDraw->GetData());
				objList[nowType][i].push_back(draw);
				greedObjs[nowType][i][j] = draw;
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

void MapEditor::SetType(string t)
{
	if (t == "TREE")
	{
		nowType = LayerType::Object;
	}
	if (t == "Tile")
	{
		nowType = LayerType::Tile;
	}
}

void MapEditor::Save()
{
	saveObjs.clear();
	for (auto& layer : greedObjs)
	{
		for (auto& objs : layer.second)
		{
			for (auto& obj : objs.second)
			{
				auto& nowObject = obj.second;
				ObjectData data;
				data.type = nowObject->GetType();
				data.path = nowObject->GetPath();
				data.uiPath = nowObject->GetData().uiPaht;
				data.position = nowObject->GetPos();
				saveObjs.push_back(data);
			}
		}
	}
	FILE_MGR->SaveMap(saveObjs, "Tutorial");
}

void MapEditor::Load()
{
	auto data = FILE_MGR->GetMap("Tutorial");
	for (auto& obj : data)
	{
		DrawObj* draw = new DrawObj(uiMgr);
		draw->SetType(obj.type);
		draw->SetPath(obj.path);
		draw->SetTexture(*RESOURCES_MGR->GetTexture(draw->GetPath()), true);
		draw->SetOrigin(Origins::BC);
		draw->SetMove(false);
		draw->SetPos(obj.position);
		draw->SetData({ obj.path, obj.uiPath });
		
		int i = ((int)obj.position.x-30) / 60;
		int j = (int)obj.position.y / 60 - 1;
		if (obj.type == "TREE")
		{
			objList[LayerType::Object][j].push_back(draw);
			greedObjs[LayerType::Object][j][i] = draw;

			cout << i << endl;
			cout << j << endl << endl;
		}
		if (obj.type == "Tile")
		{
			objList[LayerType::Tile][j].push_back(draw);
			greedObjs[LayerType::Tile][j][i] = draw;

			cout << i << endl;
			cout << j << endl << endl;
		}
	}
}
