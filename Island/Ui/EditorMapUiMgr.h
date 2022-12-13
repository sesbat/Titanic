#pragma once
#include "UiMgr.h"

class AddItemBox;
class Button;
class DrawSelect;
class DrawObj;
class SaveWindowBox;
class LoadWindowBox;
class ConnectWindowBox;
class EditorMapUiMgr : public UiMgr
{
private:
	Button* saveBtn;
	Button* loadBtn;
	Button* eraseBtn;
	Button* exitBtn;
	Button* underUi;
	Button* boxBtn;
	Button* connecntBtn;
	Button* selectBtn;

	bool isErase;

	vector<string> selects;
	vector<int> selectTxtSize;
	vector<float> selectPosY;
	int selIdx;	
	map<string, vector<Button*>> type_selects;
	int selectIdx = 0;

	vector<DrawSelect*> drawObj;
	DrawObj* nowDraw;
	map<string, vector<EditorObjs>> editorObjs;

	SaveWindowBox* saveWindow;
	LoadWindowBox* loadWindow;
	ConnectWindowBox* connectWindow;
	AddItemBox* itemBox;

	bool isBox;
	bool boxingErase;
	RectangleShape* rect;
	Vector2f rectStartPos;

public:
	EditorMapUiMgr(Scene* scene);
	virtual ~EditorMapUiMgr();
	virtual void Release();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Select(DrawSelect* select);
	DrawObj* GetDraw() { return nowDraw; }
	void SetErase(bool state = true);
	void DeletDraw();
	bool IsUnder();
	void SetLoadPath(string path);

	void SetLoadInit();

	bool IsSave();
	//bool IsCancle();
	bool IsLoad();
	bool LoadActive();
	bool ConnectActive();
	string loadFile();
	bool IsErase() { return isErase; }
	bool IsExit();
	void CloseSaveWinow();

	void SetItemBox(bool state);
	void SetItemBox();
	AddItemBox* GetItemBox() { return itemBox; }

	string GetPath();

	void BoxingEnd();
	void SetSelectSize();
	bool GetIsBox() { return isBox; }
	ConnectWindowBox* GetConnecntWindow() { return connectWindow; }

	Button* GetUnderUi() { return underUi; }

};

