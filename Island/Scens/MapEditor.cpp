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

	DrawObj* draw = new DrawObj(uiMgr);
	auto editorObjs = FILE_MGR->GetEditorObjs();
	auto playerData = editorObjs["PLAYER"];
	draw->SetType("PLAYER");
	draw->SetPath(playerData[0].texPath);
	draw->SetTexture(*RESOURCES_MGR->GetTexture(draw->GetPath()), true);
	draw->SetOrigin(Origins::BC);
	draw->SetMove(false);
	draw->SetPos(greeds[0][0]->GetPos() + Vector2f{ 30.f, 60.f });
	draw->SetData(playerData[0]);
	objList[nowType][0].push_back(draw);
	greedObjs[nowType][0][0] = draw;

	player = draw;
}

void MapEditor::Update(float dt)
{
	Scene::Update(dt);
	
	auto uimgr = ((EditorMapUiMgr*)uiMgr);


	if (uimgr->IsExit() || InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
		return;
	}

	if (uimgr->IsSave())
	{
		Save();
		return;
	}

	if (uimgr->IsLoad())
	{
		string path = uimgr->loadFile();
		Load(path);

		return;
	}

	if (uimgr->IsErase() || InputMgr::GetKeyDown(Keyboard::E))
	{
		((EditorMapUiMgr*)uiMgr)->DeleteDraw();
		return;
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
		auto movePos = SCENE_MGR->GetCurrScene()->GetWorldView().getCenter() - deltaPos;
		SCENE_MGR->GetCurrScene()->GetWorldView().setCenter(movePos);
	}
	if (InputMgr::GetMouseButtonUp(Mouse::Right))
	{
		isMove = false;
	}

	
	if (InputMgr::GetMouseWheelUp())
	{
		if (!((EditorMapUiMgr*)uiMgr)->LoadActive())
			SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() - (Vector2f{19.2,10.8} * 3.f));
	}
	if (InputMgr::GetMouseWheelDown())
	{
		if (!((EditorMapUiMgr*)uiMgr)->LoadActive())
			SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() + (Vector2f{ 19.2,10.8 } *3.f));
	}

	if (uimgr->GetEvent())
	{
		return;
	}
	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			if (greeds[i][j]->IsClick())
			{
				if(((EditorMapUiMgr*)uiMgr)->IsUnder())
					return;
				if (nowType == LayerType::Object &&playerPos == Vector2i{ i,j })
					return;

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

						if (nowDraw->GetType() == "PLAYER")
							return;

						auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
						objList[nowType][i].erase(deleteObj);
						greedObjs[nowType][i].erase(nowGreedObjs[i].find(j));

						delete findObj;
					}
				}

				DrawObj* draw = new DrawObj(uiMgr);
				draw->SetType(nowDraw->GetType());
				draw->SetPath(nowDraw->GetPath());
				draw->SetTexture(*RESOURCES_MGR->GetTexture(draw->GetPath()), true);
				draw->SetOrigin(Origins::BC);
				draw->SetMove(false);
				draw->SetPos(greeds[i][j]->GetPos() + Vector2f{ 30.f, 60.f });
				draw->SetData(nowDraw->GetData());
				objList[nowType][i].push_back(draw);
				greedObjs[nowType][i][j] = draw;

				if (nowDraw->GetType() == "PLAYER")
				{
					if (player != nullptr)
					{
						int pi = playerPos.x;
						int pj = playerPos.y;
						if (nowGreedObjs.find(pi) != nowGreedObjs.end())
						{
							if (nowGreedObjs[pi].find(pj) != nowGreedObjs[pi].end())
							{
								findObj = nowGreedObjs[pi][pj];
								auto deleteObj = find(objList[nowType][pi].begin(), objList[nowType][pi].end(), findObj);
								objList[nowType][pi].erase(deleteObj);
								greedObjs[nowType][pi].erase(nowGreedObjs[pi].find(pj));

								delete findObj;
							}
						}
					}
					player = draw;
					playerPos = { i,j };
				}
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
	Release();
}

void MapEditor::Release()
{
	for (auto& objs : objList[LayerType::Object])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}

	for (auto& objs : objList[LayerType::Tile])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}
	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	greedObjs.clear();

	player = nullptr;

}

MapEditor::~MapEditor()
{
}

void MapEditor::SetType(string t)
{
	if (t == "TREE" || t == "STONE" || t == "ENEMY" || t == "PLAYER" || t == "BLOCK")
	{
		nowType = LayerType::Object;
	}
	if (t == "TILE")
	{
		nowType = LayerType::Tile;
	}
}

void MapEditor::Save()
{
	saveObjs.clear();
	string path = ((EditorMapUiMgr*)(uiMgr))->GetPath();
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
				data.position = nowObject->GetPos();
				saveObjs.push_back(data);
			}
		}
	}
	

	if (path == "")
		return;

	FILE_MGR->SaveMap(saveObjs, path);
	((EditorMapUiMgr*)uiMgr)->SetLoadInit();
}

void MapEditor::Load(string path)
{
	for (auto& objs : objList[LayerType::Object])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}

	for (auto& objs : objList[LayerType::Tile])
	{
		for (auto it = objs.second.begin(); it != objs.second.end();)
		{
			auto del = *it;
			it = objs.second.erase(it);
			if (del != nullptr)
			{
				delete del;
			}
		}
		objs.second.clear();
	}
	objList[LayerType::Tile].clear();
	objList[LayerType::Object].clear();
	greedObjs.clear();

	player = nullptr;
	auto& data = FILE_MGR->GetMap(path);
	for (auto& obj : data)
	{
		DrawObj* draw = new DrawObj(uiMgr);
		draw->SetType(obj.type);
		draw->SetPath(obj.path);
		draw->SetTexture(*RESOURCES_MGR->GetTexture(draw->GetPath()), true);
		draw->SetOrigin(Origins::BC);
		draw->SetMove(false);
		draw->SetPos(obj.position);
		
		int i = ((int)obj.position.x-30) / 60;
		int j = (int)obj.position.y / 60 - 1;
		if (obj.type == "TREE" || obj.type == "STONE" || obj.type == "ENEMY" || obj.type == "PLAYER" || obj.type == "BLOCK")
		{
			objList[LayerType::Object][j].push_back(draw);
			greedObjs[LayerType::Object][j][i] = draw;

			if (obj.type == "PLAYER")
			{
				player = draw;
				playerPos = Vector2i{ j,i };
			}
		}
		else if (obj.type == "TILE")
		{
			objList[LayerType::Tile][j].push_back(draw);
			greedObjs[LayerType::Tile][j][i] = draw;
		}
	}

	((EditorMapUiMgr*)uiMgr)->SetLoadPath(path);
}
