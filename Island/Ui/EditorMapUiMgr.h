#pragma once
#include "UiMgr.h"

class Button;
class DrawSelect;
class DrawObj;
class SaveWindowBox;
class LoadWindowBox;
class EditorMapUiMgr : public UiMgr
{
private:
	Button* saveBtn;
	Button* loadBtn;
	Button* eraseBtn;
	Button* exitBtn;
	Button* underUi;

	Button* selectBtn;
	vector<string> selects;
	vector<int> selectTxtSize;
	vector<float> selectPosY;
	int selIdx;
	map<string, vector<Button*>> type_selects;

	vector<DrawSelect*> drawObj;
	DrawObj* nowDraw;
	map<string, vector<EditorObjs>> editorObjs;

	SaveWindowBox* saveWindow;
	LoadWindowBox* loadWindow;

public:
	EditorMapUiMgr(Scene* scene);
	virtual ~EditorMapUiMgr();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Select(DrawSelect* select);
	DrawObj* GetDraw() { return nowDraw; }
	void DeleteDraw();
	bool IsUnder();
	void SetLoadPath(string path);

	void SetLoadInit();

	bool IsSave();
	bool IsLoad();
	bool LoadActive();
	string loadFile();
	bool IsErase();

	bool IsExit();

	string GetPath();

};

