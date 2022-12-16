#pragma once
#include "Button.h"

class ConnectWindowBox : public Button
{
private:
	ConnectWindowBox(const ConnectWindowBox& ref) : Button(nullptr) {}
	ConnectWindowBox& operator=(const ConnectWindowBox& ref) {}
	ConnectWindowBox(ConnectWindowBox& ref) : Button(nullptr) {}
	ConnectWindowBox& operator=(ConnectWindowBox& ref) {}
	
	vector<Button*> allFile;
	map<Button*, bool> fileState;
	int idx;

public:
	ConnectWindowBox(UiMgr* mgr);
	~ConnectWindowBox();
	virtual void Release();
	void Init();
	void Reset(string name = "");
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
	vector<string> GetConnectMaps();
};