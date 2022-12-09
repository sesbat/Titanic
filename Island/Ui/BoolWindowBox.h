#pragma once
#include "Button.h"
class BoolWindowBox :
    public Button
{
private:
	BoolWindowBox(const BoolWindowBox& ref) : Button(nullptr) {}
	BoolWindowBox& operator=(const BoolWindowBox& ref) {}
	BoolWindowBox(BoolWindowBox& ref) : Button(nullptr) {}
	BoolWindowBox& operator=(BoolWindowBox& ref) {}
	Button* yes;
	Button* no;
	//Button* cancle;
	string path;
	TextObject* txt;
	TextObject* txt2;

	bool isClickYes;
	bool isClickNo;
public:
	BoolWindowBox(UiMgr* mgr);
	~BoolWindowBox();
	void Init();
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
	virtual void Release();

	void SetPath(string path);

	bool GetYes() { return isClickYes; }
	bool GetNo() { return isClickNo; }

	//virtual bool IsCancle();
	virtual string GetPath() { return path; }
};


