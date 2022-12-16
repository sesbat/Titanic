#pragma once
#include "Button.h"

class LoadWindowBox : public Button
{
private:
	LoadWindowBox(const LoadWindowBox& ref) : Button(nullptr) {}
	LoadWindowBox& operator=(const LoadWindowBox& ref) {}
	LoadWindowBox(LoadWindowBox& ref) : Button(nullptr) {}
	LoadWindowBox& operator=(LoadWindowBox& ref) {}
	vector<Button*> loadFiles;
	map<string, vector<ObjectData>> allMaps;
	int idx;
	string loadPath;

public:
	LoadWindowBox(UiMgr* mgr);
	~LoadWindowBox();
	virtual void Release();
	void Init();
	void Reset();
	bool IsLoad();
	string GetLoadPaht() { return loadPath; }
	virtual void Draw(RenderWindow& window);
	virtual void Update(float dt);
};