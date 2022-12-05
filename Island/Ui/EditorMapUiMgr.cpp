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
#include "../Ui/AddItemBox.h"
#include "SaveWindowBox.h"
#include "LoadWindowBox.h"
#include "../GameObject/TextObject.h"

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
	eraseBtn->SetClkColor(false);
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

	boxBtn = new Button(this);
	boxBtn->SetClkColor(false);
	boxBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "Box", true);
	boxBtn->SetOrigin(Origins::TL);
	boxBtn->SetPos({ 50,330 });
	uiObjList[0].push_back(boxBtn);


	selects = { "TILE","TREE","BUSH","STONE","BLOCK","PLAYER","ENEMY","BOX","ANOTHER"};
	selectTxtSize = { 75,75,75,65,65,55,60,75,40 };
	selectPosY = { 54,54,54,54,54,62,54,54,70 };

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

	itemBox = new AddItemBox(this);
	itemBox->Init();
	uiObjList[1].push_back(itemBox);
}

void EditorMapUiMgr::Reset()
{
}

void EditorMapUiMgr::Update(float dt)
{
	if (saveWindow->GetActive())
	{
		saveBtn->Update(dt);
		if (saveBtn->IsUp())
		{
			saveWindow->SetActive(!saveWindow->GetActive());
		}
		//if(saveWindow->IsCancle())
		//	saveWindow->SetActive(false);
		saveWindow->Update(dt);
		return;
	}
	if (loadWindow->GetActive())
	{
		loadBtn->Update(dt);
		if (loadBtn->IsUp())
		{
			loadWindow->SetActive(!loadWindow->GetActive());
			((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeletDraw();
		}
		loadWindow->Update(dt);
		return;
	}

	UiMgr::Update(dt);

	if (eraseBtn->IsUp())
	{
		isErase = !isErase;
		if (isErase)
		{
			SetErase();
		}
		else
			eraseBtn->GetTextObj()->SetColor(Color::White);

		boxingErase = isErase;
		return;
	}
	if (boxBtn->IsUp())
	{
		//cout << "Box Down" << endl;
		isBox = !isBox;
		if (isBox)
			boxBtn->GetTextObj()->SetColor(Color::Red);
		else
			boxBtn->GetTextObj()->SetColor(Color::White);

		boxingErase = isErase;

		return;
	}
	if (isBox && InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		if (rect != nullptr)
			delete rect;
		rect = new RectangleShape();
		rectStartPos = InputMgr::GetMousePos();
		rectStartPos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)rectStartPos);
		rect->setFillColor(Color(255, 255, 255, 20));
		rect->setPosition(rectStartPos);
		//cout << rectStartPos.x << endl;
		//cout << rectStartPos.y << endl;

		return;
	}
	if (isBox && InputMgr::GetMouseButton(Mouse::Left))
	{
		auto mousePos = InputMgr::GetMousePos();
		mousePos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)mousePos);
		rect->setSize(Vector2f{ mousePos.x - rectStartPos.x, mousePos.y - rectStartPos.y });
		//cout << "Box ing" << endl;

		return;
	}
	if (isBox && InputMgr::GetMouseButtonUp(Mouse::Left))
	{
		BoxingEnd();
		//cout << "Box End" << endl;
		return;
	}

	if (nowDraw != nullptr)
	{
		nowDraw->Update(dt);
	}

	if (saveBtn->IsUp())
	{
		saveWindow->SetActive(!saveWindow->GetActive());
		//saveWindow->SetActive(true);
		((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeletDraw();
	}
	if (loadBtn->IsUp())
	{
		loadWindow->SetActive(!loadWindow->GetActive());
		((EditorMapUiMgr*)(parentScene->GetUiMgr()))->DeletDraw();
	}

	if (selectBtn->IsUp())
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

		DeletDraw();
		((MapEditor*)(parentScene))->SetType(selects[selIdx]);
	}

	if (selectBtn->IsUpRight())
	{
		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(false);
		}
		selIdx = (selects.size() + selIdx - 1) % selects.size();
		selectBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			selectTxtSize[selIdx], Color::White, selects[selIdx], true); //TILE TREE STONE PLAYER ENEMY BOX ANOTHER
		selectBtn->SetOrigin(Origins::MC);
		selectBtn->SetPos(underUi->GetPos() + Vector2f{ 205, selectPosY[selIdx]});

		for (auto& obj : type_selects[selects[selIdx]])
		{
			obj->SetActive(true);
		}

		DeletDraw();
		((MapEditor*)(parentScene))->SetType(selects[selIdx]);
	}

}

void EditorMapUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
	if (nowDraw != nullptr)
		nowDraw->Draw(window);
	if (rect != nullptr)
		window.draw(*rect);
}

void EditorMapUiMgr::Select(DrawSelect* select)
{
	if (IsErase())
	{
		((MapEditor*)parentScene)->SetType(select->GetType());
		return;
	}

	if(nowDraw == nullptr)
		nowDraw = new DrawObj(this);
	nowDraw->SetTexture(*RESOURCES_MGR->GetTexture(select->GetTexturePath()), true);
	nowDraw->SetOrigin(Origins::BC);
	nowDraw->SetType(select->GetType());
	nowDraw->SetPath(select->GetPath());
	nowDraw->SetData(select->GetData());
	((MapEditor*)parentScene)->SetType(select->GetType());
}

void EditorMapUiMgr::SetErase(bool state)
{
	if (state)
	{
		delete nowDraw;
		nowDraw = nullptr;
		isErase = true;

		eraseBtn->GetTextObj()->SetColor(Color::Red);
	}
	else
	{
		isErase = false;
		eraseBtn->GetTextObj()->SetColor(Color::White);
	}
}

void EditorMapUiMgr::DeletDraw()
{
	delete nowDraw;
	nowDraw = nullptr;
}


bool EditorMapUiMgr::IsUnder()
{
	auto mousePos = InputMgr::GetMousePos();
	mousePos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)mousePos);
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

//bool EditorMapUiMgr::IsCancle()
//{
//	return saveWindow->IsCancle();
//}

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

bool EditorMapUiMgr::IsExit()
{
	return exitBtn->IsDown() || exitBtn->IsClick();
}

void EditorMapUiMgr::CloseSaveWinow()
{
	saveWindow->SetActive(false);
}

void EditorMapUiMgr::SetItemBox(bool state)
{
	itemBox->SetActive(state);
}

void EditorMapUiMgr::SetItemBox()
{
	itemBox->SetActive(!itemBox->GetActive());
}

string EditorMapUiMgr::GetPath()
{
	return saveWindow->GetPath();
}

void EditorMapUiMgr::BoxingEnd()
{
	auto rectPos = rect->getPosition();;
	rectPos = SCENE_MGR->GetCurrScene()->ScreenToWorld((Vector2i)rectPos);
	auto rectBound = rect->getGlobalBounds();
	float size_x = SCENE_MGR->GetCurrScene()->GetWorldView().getSize().x;
	float scale = size_x / 1920.f;

	int start_idx_x = (rectPos.x) / 60;
	int start_idx_y = (rectPos.y) / 60;

	int end_idx_x = ((rectPos.x + rectBound.width * scale)) / 60;
	int end_idx_y = ((rectPos.y + rectBound.height * scale)) / 60;

	start_idx_x = max(0,min(start_idx_x, 128));
	start_idx_y = max(0, min(start_idx_y, 72));
	end_idx_x = max(0, min(end_idx_x, 128));
	end_idx_y = max(0, min(end_idx_y, 72));

	for (int i = start_idx_x; i < end_idx_x; i++)
	{
		for (int j = start_idx_y; j < end_idx_y; j++)
		{

			((MapEditor*)parentScene)->DrawBox(j, i);
		}

	}

	/*if (boxingErase)
	{
		cout << "Erase" << endl;
		cout << start_idx_x << endl;
		cout << start_idx_y << endl;
		cout << end_idx_x << endl;
		cout << end_idx_y << endl;
	}
	else
	{
		cout << "draw" << endl;
		cout << start_idx_x << endl;
		cout << start_idx_y << endl;
		cout << end_idx_x << endl;
		cout << end_idx_y << endl;
	}*/


	if (rect != nullptr)
		delete rect;

	rect = nullptr;


}