#pragma once
#include "UiMgr.h"

class Button;
class DrawSelect;
class DrawObj;
class EditorMapUiMgr : public UiMgr
{
private:
	Button* underUi;
	vector<DrawSelect*> drawObj;
	DrawObj* nowDraw;
	map<string, vector<EditorObjs>> editorObjs;

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

};

