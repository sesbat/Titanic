#pragma once
#include "Button.h"
class SaveWindowBox : public Button 
{
private:
	Button* save;
	Button* cancle;
	string path;
public:
	SaveWindowBox(UiMgr* mgr);
	~SaveWindowBox();
	void Init();
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);

	virtual bool IsSave();
	virtual bool IsCancle();
};

