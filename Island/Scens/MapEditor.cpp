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
#include "../Ui/ConnectWindowBox.h"
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
			greedObjs[LayerType::Back][0][0] = nullptr;
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
	playerPos = Vector2i{ 0,0 };
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
	greedObjs[nowType][0][1] = exit;
	exitPos = Vector2i{ 0,1 };

	now_exit = exit;

}

void MapEditor::Update(float dt)
{
	auto uimgr = ((EditorMapUiMgr*)uiMgr);

	Vector2i cam_pos = (Vector2i)GetWorldView().getCenter();
	Vector2i cam_size = (Vector2i)GetWorldView().getSize();
	cam_pos = cam_pos - Vector2i{ cam_size.x / 2, cam_size.y / 2 };
	cam_pos /= 60;

	view_start_pos = cam_pos + Vector2i{-1,-1};
	veiw_end_pos = Vector2i{ cam_pos.x + (cam_size.x / 60) +1 ,cam_pos.y + (cam_size.y / 60)+1 };

	view_start_pos.x = max(view_start_pos.x, 0);
	view_start_pos.y = max(view_start_pos.y, 0);
	veiw_end_pos.x = min(veiw_end_pos.x, 128);
	veiw_end_pos.y = min(veiw_end_pos.y, 72);

	for (int i = view_start_pos.x; i < veiw_end_pos.x; i++)
	{
		for (int j = view_start_pos.y; j < veiw_end_pos.y; j++)
		{
			greeds[j][i]->Update(dt);
			if (greedObjs[LayerType::Object][j][i] != nullptr)
				greedObjs[LayerType::Object][j][i]->Update(dt);
		}
	}

	if (uiMgr != nullptr)
		uiMgr->Update(dt);

	if (uimgr->IsExit() || InputMgr::GetKeyDown(Keyboard::Escape))
	{
		SCENE_MGR->ChangeScene(Scenes::Menu);
		return;
	}

	if (uimgr->IsSave())
	{
		std::cout << "save" << endl;
		Save();
		return;
	}

	if (uimgr->IsLoad())
	{
		string path = uimgr->loadFile();
		Load(path);

		return;
	}

	if (InputMgr::GetKeyDown(Keyboard::E))
	{
		((EditorMapUiMgr*)uiMgr)->SetErase(!((EditorMapUiMgr*)uiMgr)->IsErase());
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
		if (!((EditorMapUiMgr*)uiMgr)->GetUnderUi()->IsStay())
		if (!((EditorMapUiMgr*)uiMgr)->GetItemBox()->GetActive())
			if (!((EditorMapUiMgr*)uiMgr)->LoadActive() && !((EditorMapUiMgr*)uiMgr)->ConnectActive())
				SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() - (Vector2f{ 19.2,10.8 } *10.f));
	}
	if (InputMgr::GetMouseWheelDown())
	{
		if (!((EditorMapUiMgr*)uiMgr)->GetUnderUi()->IsStay())
		if (!((EditorMapUiMgr*)uiMgr)->GetItemBox()->GetActive())
			if (!((EditorMapUiMgr*)uiMgr)->LoadActive() && !((EditorMapUiMgr*)uiMgr)->ConnectActive())
				SCENE_MGR->GetCurrScene()->GetWorldView().setSize(SCENE_MGR->GetCurrScene()->GetWorldView().getSize() + (Vector2f{ 19.2,10.8 } *10.f));
	}

	if (uimgr->GetEvent())
	{
		return;
	}

	if (uimgr->GetIsBox())
		return;


	for (int i = view_start_pos.y; i < veiw_end_pos.y; i++)
	{
		for (int j = view_start_pos.x; j < veiw_end_pos.x; j++)
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
				if (greedObjs[nowType][i][j] == nullptr)
					continue;
				if ((greedObjs[nowType][i][j]->GetType() != "ENEMY") && (greedObjs[nowType][i][j]->GetType() != "BOSS") && (greedObjs[nowType][i][j]->GetType() != "BOX"))
					continue;

				auto itemBox = ((EditorMapUiMgr*)uimgr)->GetItemBox();
				itemBox->SetActive(!itemBox->GetActive());
				itemBox->SetItems(greedObjs[nowType][i][j]->GetItem());

			}
		}
	}
}

void MapEditor::Draw(RenderWindow& window)
{
	window.setView(worldView);

	for (int i = view_start_pos.x; i <veiw_end_pos.x; i++)
	{
		for (int j = view_start_pos.y; j < veiw_end_pos.y; j++)
		{
			greeds[j][i]->Draw(window);

			if(greedObjs[LayerType::Tile][j][i] != nullptr)
				greedObjs[LayerType::Tile][j][i]->Draw(window);
		}
	}

	for (int j = view_start_pos.y; j < veiw_end_pos.y; j++)
	{
		for (int i = view_start_pos.x; i < veiw_end_pos.x; i++)
		{
			if (greedObjs[LayerType::Object][j][i] != nullptr)
				greedObjs[LayerType::Object][j][i]->Draw(window);
		}
	}

	////for (auto objs : greedObjs[LayerType::Object])
	////{
	////	for (auto obj : objs.second)
	////	{
	////		obj.second->Draw(window);
	////	}
	////}
	if (uiMgr != nullptr)
		uiMgr->Draw(window);
	//Scene::Draw(window);
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
	Release();
}

void MapEditor::SetType(string t)
{
	if (t == "TREE" || t == "BUSH" || t == "STONE" || t == "ENEMY" || t == "BOSS" || t == "PLAYER" ||
		t == "BLOCK" || t == "ANOTHER" || t == "BOX" || t == "RADIATION" || t == "INVISIBLE" || t == "RADTILE")
	{
		nowType = LayerType::Object;
	}
	if (t == "TILE" )
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
				if (obj.second == nullptr)
					continue;
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
	FILE_MGR->SaveConnecnt(path,((EditorMapUiMgr*)uiMgr)->GetConnecntWindow()->GetConnectMaps());
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
		if (obj.type == "TREE" || obj.type == "BUSH" || obj.type == "STONE" || obj.type == "ENEMY" || obj.type == "BOSS"||
			obj.type == "PLAYER" || obj.type == "BLOCK" || obj.type == "ANOTHER" || obj.type == "BOX" ||
			obj.type == "RADIATION"|| obj.type == "INVISIBLE" || obj.type == "RADTILE")
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
		else if (obj.type == "TILE" )
		{
			objList[LayerType::Tile][j].push_back(draw);
			greedObjs[LayerType::Tile][j][i] = draw;
		}
	}

	((EditorMapUiMgr*)uiMgr)->SetLoadPath(path);
	((EditorMapUiMgr*)uiMgr)->GetConnecntWindow()->Reset(path);


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

		if (((EditorMapUiMgr*)uiMgr)->IsErase() || ((EditorMapUiMgr*)uiMgr)->IsUnder())
		{
			Button* findObj = nullptr;
			if (nowGreedObjs[i][j] != nullptr)
			{
				findObj = nowGreedObjs[i][j];
				auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
				objList[nowType][i].erase(deleteObj);
				greedObjs[nowType][i][j] = nullptr;

				delete findObj;
			}
			return false;
		}
		if (nowDraw == nullptr)
		{
			return true;
		}
		Button* findObj = nullptr;
		if (nowGreedObjs[i][j] != nullptr)
		{
			findObj = nowGreedObjs[i][j];

			if (nowDraw->GetType() == "PLAYER")
				return false;

			auto deleteObj = find(objList[nowType][i].begin(), objList[nowType][i].end(), findObj);
			objList[nowType][i].erase(deleteObj);
			greedObjs[nowType][i][j] = nullptr;

			delete findObj;
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
				if (nowGreedObjs[pi][pj] != nullptr)
				{
					findObj = nowGreedObjs[pi][pj];
					auto deleteObj = find(objList[nowType][pi].begin(), objList[nowType][pi].end(), findObj);
					objList[nowType][pi].erase(deleteObj);
					greedObjs[nowType][pi][pj] = nullptr;

					delete findObj;
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
				if (nowGreedObjs[ei][ej] != nullptr)
				{
					findObj = nowGreedObjs[ei][ej];
					auto deleteObj = find(objList[nowType][ei].begin(), objList[nowType][ei].end(), findObj);
					objList[nowType][ei].erase(deleteObj);
					greedObjs[nowType][ei][ej] = nullptr;

					delete findObj;
				}
			}
			now_exit = draw;
			exitPos = { i,j };
		}

		return true;
}