#pragma once
#include "Button.h"
class DrawSelect : public Button
{
private:
	DrawSelect(const DrawSelect& ref) : Button(nullptr) {}
	DrawSelect& operator=(const DrawSelect& ref) {}
	DrawSelect(DrawSelect& ref) : Button(nullptr) {}
	DrawSelect& operator=(DrawSelect& ref) {}
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
	virtual void Release();
};

