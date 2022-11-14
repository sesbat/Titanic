#pragma once
#include "Button.h"
class DrawSelect : public Button
{
private:
	string type;
	string path;
	string drawPath;
	EditorObjs data;
public:
	DrawSelect(UiMgr* uimgr);
	~DrawSelect();

	virtual void Set(string type, string path, string drawPath);
	virtual void Update(float dt);
	string GetTexturePath() { return path; }
	string GetType() { return type; }
	string GetPath() { return path; }
	void SetData(EditorObjs d) { data = d; }
	EditorObjs GetData() { return data; }
};

