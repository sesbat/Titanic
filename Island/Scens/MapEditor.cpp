#include "MapEditor.h"
#include "../Ui/Button.h"
#include "../Framework/ResourceManager.h"
#include "../Scens/SceneManager.h"
#include "../Framework/info.h"
#include "../../Ui/EditorMapUiMgr.h"
#include "../../Ui/DrawObj.h"
#include "../../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../Ui/AddItemBox.h"
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

	DrawObj* exit = new DrawObj(uiMgr);
	auto exitData = editorObjs["ANOTHER"];
	exit->SetType("ANOTHER");
	exit->SetPath(exitData[0].texPath);
	exit->SetTexture(*RESOURCES_MGR->GetTexture(exit->GetPath()), true);
	exit->SetOrigin(Origins::BC);
	exit->SetMove(false);
	exit->SetPos(greeds[0][1]->GetPos() + Vector2f{ 30.f, 60.f });
	exit->SetData(exitData[0]);
	objList[nowType][0].push_back(exit);
	greedObjs[nowType][0][0] = exit;

	now_exit = exit;

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
		std::cout << "save" << endl;
		Save();
		//uimgr->CloseSaveWinow();
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
		if (!((EditorMapUiMgr*)uiMgr)->GetItemBox()->GetActive())
			if (!((EditorMapUiMgr*)uiMgr)->LoadActive())
				SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() - (Vector2f{ 19.2,10.8 } *3.f));
	}
	if (InputMgr::GetMouseWheelDown())
	{
		if (!((EditorMapUiMgr*)uiMgr)->GetItemBox()->GetActive())
			if (!((EditorMapUiMgr*)uiMgr)->LoadActive())
				SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() + (Vector2f{ 19.2,10.8 } *3.f));
	}

	if (uimgr->GetEvent())
	{
		return;
	}

	if (uimgr->GetIsBox())
		return;

	for (int i = 0; i < HEIGHTCNT; i++)
	{
		for (int j = 0; j < WIDTHCNT; j++)
		{
			if (greeds[i][j]->IsClick())
			{
				if(!DrawBox(i, j))
					return;
			}
			else if (greeds[i][j]->IsUpRight())
			{
				if (nowType == LayerType::Tile)
					continue;
				if (greedObjs[nowType].find(i) == greedObjs[nowType].end())
					continue;
				if (greedObjs[nowType][i].find(j) == greedObjs[nowType][i].end())
					continue;
				if ((greedObjs[nowType][i][j]->GetType() != "ENEMY") && (greedObjs[nowType][i][j]->GetType() != "BOX"))
					continue;

				auto itemBox = ((EditorMapUiMgr*)uimgr)->GetItemBox();
				itemBox->SetActive(!itemBox->GetActive());
				itemBox->SetItems(greedObjs[nowType][i][j]->GetItem());

				cout << i << endl;
				cout << j << endl;
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
	now_exit = nullptr;

}

MapEditor::~MapEditor()
{
}

void MapEditor::SetType(string t)
{
	if (t == "TREE" || t == "BUSH" || t == "STONE" || t == "ENEMY" || t == "PLAYER" ||
		t == "BLOCK" || t == "ANOTHER" || t == "BOX")
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
			int i = objs.first;
			for (auto& obj : objs.second)
			{
				int j = obj.first;
				auto& nowObject = obj.second;
				ObjectData data;
				data.type = nowObject->GetType();
				data.path = nowObject->GetPath();
				data.position = nowObject->GetPos();
				data.greedIdx = Vector2i{ i,j };
				data.item = nowObject->GetItem();
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
	now_exit = nullptr;
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
		draw->SetItem(obj.item);

		int i = ((int)obj.position.x - 30) / 60;
		int j = (int)obj.position.y / 60 - 1;
		if (obj.type == "TREE" || obj.type == "BUSH" || obj.type == "STONE" || obj.type == "ENEMY" ||
			obj.type == "PLAYER" || obj.type == "BLOCK" || obj.type == "ANOTHER" || obj.type == "BOX")
		{
			objList[LayerType::Object][j].push_back(draw);
			greedObjs[LayerType::Object][j][i] = draw;

			if (obj.type == "PLAYER")
			{
				player = draw;
				playerPos = Vector2i{ j,i };
			}
			if (obj.type == "ANOTHER")
			{
				now_exit = draw;
				exitPos = Vector2i{ j,i };
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

bool MapEditor::DrawBox(int i, int j)
{
		if (((EditorMapUiMgr*)uiMgr)->IsUnder())
			return false;
		if (nowType == LayerType::Object && playerPos == Vector2i{ i,j })
			return false;
		if (nowType == LayerType::Object && exitPos == Vector2i{ i,j })
			return false;

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
			return false;
		}

		Button* findObj = nullptr;
		if (nowGreedObjs.find(i) != nowGreedObjs.end())
		{
			if (nowGreedObjs[i].find(j) != nowGreedObjs[i].end())
			{
				findObj = nowGreedObjs[i][j];

				if (nowDraw->GetType() == "PLAYER")
					return false;

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
		if (nowDraw->GetType() == "ANOTHER")
		{
			if (now_exit != nullptr)
			{
				int ei = exitPos.x;
				int ej = exitPos.y;
				if (nowGreedObjs.find(ei) != nowGreedObjs.end())
				{
					if (nowGreedObjs[ei].find(ej) != nowGreedObjs[ei].end())
					{
						findObj = nowGreedObjs[ei][ej];
						auto deleteObj = find(objList[nowType][ei].begin(), objList[nowType][ei].end(), findObj);
						objList[nowType][ei].erase(deleteObj);
						greedObjs[nowType][ei].erase(nowGreedObjs[ei].find(ej));

						delete findObj;
					}
				}
			}
			now_exit = draw;
			exitPos = { i,j };
		}

		return true;
}