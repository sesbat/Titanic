#pragma once
#include "Button.h"

class TextObject;
class SaveWindowBox : public Button 
{
private:
	Button* save;
	Button* cancle;
	string path;
	TextObject* txt;
public:
	SaveWindowBox(UiMgr* mgr);
	~SaveWindowBox();
	void Init();
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);

	virtual bool IsSave();
	virtual bool IsCancle();
	virtual string GetPath() { return path ; }
};

