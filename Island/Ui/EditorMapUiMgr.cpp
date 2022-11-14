#include "EditorMapUiMgr.h"
#include "Button.h"
#include "DrawSelect.h"
#include "DrawObj.h"
#include "../Framework/ResourceManager.h"
#include "../Framework//info.h"
#include "../GameObject/SpriteObject.h"
#include "../Framework/InputMgr.h"
#include "../../Scens/SceneManager.h"
#include "../../Framework/FileManager.h"
#include "../../Scens/MapEditor.h"
#include "SaveWindowBox.h"
#include "LoadWindowBox.h"

EditorMapUiMgr::EditorMapUiMgr(Scene* scene)
	:UiMgr(scene)
{
}

EditorMapUiMgr::~EditorMapUiMgr()
{
}

void EditorMapUiMgr::Init()
{
	underUi = new Button(this);
	underUi->SetTexture(*RESOURCES_MGR->GetTexture("graphics/Editor/underUi.png"), true);
	underUi->SetPos({ 0,WINDOW_HEIGHT - underUi->GetSpriteObj()->GetGlobalBound().height });
	uiObjList[0].push_back(underUi);

	editorObjs = FILE_MGR->GetEditorObjs();

	saveBtn = new Button(this);
	saveBtn->SetClkColor(true);
	saveBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "SAVE", true);
	saveBtn->SetOrigin(Origins::TL);
	saveBtn->SetPos({50,50 });
	uiObjList[0].push_back(saveBtn);


	loadBtn = new Button(this);
	loadBtn->SetClkColor(true);
	loadBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "LOAD", true);
	loadBtn->SetOrigin(Origins::TL);
	loadBtn->SetPos({ 50,120 });
	uiObjList[0].push_back(loadBtn);

	eraseBtn = new Button(this);
	eraseBtn->SetClkColor(true);
	eraseBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "ERASE", true);
	eraseBtn->SetOrigin(Origins::TL);
	eraseBtn->SetPos({ 50,190 });
	uiObjList[0].push_back(eraseBtn);

	exitBtn = new Button(this);
	exitBtn->SetClkColor(true);
	exitBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "EXIT", true);
	exitBtn->SetOrigin(Origins::TL);
	exitBtn->SetPos({ 50,260 });
	uiObjList[0].push_back(exitBtn);

	selects = { "TILE","TREE","STONE","BLOCK","PLAYER","ENEMY","BOX","ANOTHER"};
	selectTxtSize = { 75,75,65,65,55,60,75,40 };
	selectPosY = { 54,54,54,54,62,54,54,70 };

	selIdx = 0;
	selectBtn = new Button(this);
	selectBtn->SetClkColor(true);
	selectBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		selectTxtSize[selIdx], Color::White, selects[selIdx], true); //TILE TREE STONE PLAYER ENEMY BOX ANOTHER
	selectBtn->SetOrigin(Origins::MC);
	selectBtn->SetPos(underUi->GetPos() + Vector2f{ 205, selectPosY[selIdx] });
	uiObjList[0].push_back(selectBtn);

	for (auto& type : editorObjs)
	{
		int x = 450;
		for (auto& obj : type.second)
		{
			DrawSelect* draw = new DrawSelect(this);
			drawObj.push_back(draw);
			draw->Set(type.first, obj.texPath, obj.uiPaht);
			draw->SetPos(underUi->GetPos() + Vector2f{ (float)x, 40.f });
			x += 100;
			draw->SetData(obj);
			uiObjList[0].push_back(draw);
			type_selects[type.first].push_back(draw);
			draw->SetActive(false);
		}
	}

	saveWindow = new SaveWindowBox(this);
	saveWindow->SetPos({ 350,50 });
	saveWindow->Init();
	uiObjList[1].push_back(saveWindow);
	for (auto& obj : type_selects[selects[selIdx]])
	{
		obj->SetActive(true);
	}

	loadWindow = new LoadWindowBox(this);
	loadWindow->SetPos({ 350,50 });
	loadWindow->Init();
	uiObjList[1].push_back(loadWindow);
}

void EditorMapUiMgr::Reset()
{
}

void EditorMapUiMgr::Update(float dt)
{
	if (saveWindow->GetActive())
	{
		saveWindow->Update(dt);
		if(saveWindow->IsCancle())
			saveWindow->SetActive(false);
		return;
	}
	if (loadWindow->GetActive())
	{
		loadBtn->Update(dt);
		if (loadBtn->IsUp())
		{
			loadWindow->SetActive(!loadWindow->GetActive());
			((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
		}
		loadWindow->Update(dt);
		return;
	}
	UiMgr::Update(dt);
	if (nowDraw != nullptr)
	{
		nowDraw->Update(dt);
	}

	cout << (int)saveBtn->GetState() << endl;
	if (saveBtn->IsUp())
	{
		saveWindow->SetActive(true);
		((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}
	if (loadBtn->IsUp())
	{
		loadWindow->SetActive(!loadWindow->GetActive());
		((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}

	if (selectBtn->IsClick())
	{
		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(false);
		}
		selIdx = (selects.size() + selIdx + 1) % selects.size();
		selectBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			selectTxtSize[selIdx], Color::White, selects[selIdx], true); //TILE TREE STONE PLAYER ENEMY BOX ANOTHER
		selectBtn->SetOrigin(Origins::MC);
		selectBtn->SetPos(underUi->GetPos() + Vector2f{ 205, selectPosY[selIdx]});

		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(true);
		}

		((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeleteDraw();
	}

}

void EditorMapUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
	if (nowDraw != nullptr)
		nowDraw->Draw(window);
}

void EditorMapUiMgr::Select(DrawSelect* select)
{
	if(nowDraw == nullptr)
		nowDraw = new DrawObj(this);
	nowDraw->SetTexture(*RESOURCES_MGR->GetTexture(select->GetTexturePath()), true);
	nowDraw->SetOrigin(Origins::BC);
	nowDraw->SetType(select->GetType());
	nowDraw->SetPath(select->GetPath());
	nowDraw->SetData(select->GetData());
	((MapEditor*)parentScene)->SetType(select->GetType());
}

void EditorMapUiMgr::DeleteDraw()
{
	delete nowDraw;
	nowDraw = nullptr;
}

bool EditorMapUiMgr::IsUnder()
{
	auto mousePos = InputMgr::GetMousePos();
	mousePos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)mousePos);
	cout << Utils::IsRange(underUi->GetSpriteObj()->GetGlobalBound(), mousePos) << endl;
	return Utils::IsRange(underUi->GetSpriteObj()->GetGlobalBound(), mousePos);
}

void EditorMapUiMgr::SetLoadPath(string path)
{
	saveWindow->SetPath(path);
}
void EditorMapUiMgr::SetLoadInit()
{
	loadWindow->Reset();
}


bool EditorMapUiMgr::IsSave()
{
	return saveWindow->IsSave();
}

bool EditorMapUiMgr::IsLoad()
{
	return loadWindow->IsLoad();
}

bool EditorMapUiMgr::LoadActive()
{
	return loadWindow->GetActive();
}

string EditorMapUiMgr::loadFile()
{
	return loadWindow->GetLoadPaht();
}

bool EditorMapUiMgr::IsErase()
{
	return eraseBtn->IsDown() || eraseBtn->IsClick();
}

bool EditorMapUiMgr::IsExit()
{
	return exitBtn->IsDown() || exitBtn->IsClick();
}

string EditorMapUiMgr::GetPath()
{
	return saveWindow->GetPath();
}