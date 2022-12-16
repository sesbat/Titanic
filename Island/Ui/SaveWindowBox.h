#pragma once
#include "Button.h"

class TextObject;
class SaveWindowBox : public Button 
{
private:
	SaveWindowBox(const SaveWindowBox& ref) : Button(nullptr) {}
	SaveWindowBox& operator=(const SaveWindowBox& ref) {}
	SaveWindowBox(SaveWindowBox& ref) : Button(nullptr) {}
	SaveWindowBox& operator=(SaveWindowBox& ref) {}
	Button* save;
	//Button* cancle;
	string path;
	TextObject* txt;
public:
	SaveWindowBox(UiMgr* mgr);
	~SaveWindowBox();
	void Init();
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
	virtual void Release();

	void SetPath(string path);

	virtual bool IsSave();
	//virtual bool IsCancle();
	virtual string GetPath() { return path ; }
};

