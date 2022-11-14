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
	int x = 20;

	for (auto& type : editorObjs)
	{
		for (auto& obj : type.second)
		{
			DrawSelect* draw = new DrawSelect(this);
			drawObj.push_back(draw);
			draw->Set(type.first, obj.texPath, obj.uiPaht);
			draw->SetPos(underUi->GetPos() + Vector2f{ (float)x, 10.f });
			x += 100;
			draw->SetData(obj);
			uiObjList[0].push_back(draw);
		}
	}

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

	//DrawSelect* draw = new DrawSelect(this);
	//drawObj.push_back(draw);
	//draw->Set("TREE", "graphics/editor/tree1.png", "graphics/editor/drawTree1.png");
	//draw->SetPos(underUi->GetPos() + Vector2f { 10, 10 });
	//uiObjList[0].push_back(draw);


	//draw = new DrawSelect(this);
	//drawObj.push_back(draw);
	//draw->Set("TREE", "graphics/editor/tree2.png", "graphics/editor/drawTree2.png");
	//draw->SetPos(underUi->GetPos() + Vector2f{ 120, 10 });
	//uiObjList[0].push_back(draw);
	//Reset();

}

void EditorMapUiMgr::Reset()
{
}

void EditorMapUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	if (nowDraw != nullptr)
		nowDraw->Update(dt);

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

bool EditorMapUiMgr::IsSave()
{
	return saveBtn->IsDown() || saveBtn->IsClick();
}

bool EditorMapUiMgr::IsLoad()
{
	return loadBtn->IsDown() || loadBtn->IsClick();
}

bool EditorMapUiMgr::IsErase()
{
	return eraseBtn->IsDown() || eraseBtn->IsClick();
}

bool EditorMapUiMgr::IsExit()
{
	return exitBtn->IsDown() || exitBtn->IsClick();
}