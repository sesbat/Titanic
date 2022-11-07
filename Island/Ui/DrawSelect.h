#pragma once
#include "Button.h"
class DrawSelect : public Button
{
private:
	string type;
	string path;
public:
	DrawSelect(UiMgr* uimgr);
	~DrawSelect();

	virtual void Set(string type, string path); 
	virtual void Update(float dt);
	string GetTexturePath() { return path; }
	string GetType() { return type; }
	string GetPath() { return path; }
};

