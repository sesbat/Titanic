#pragma once
#include "MoveObject.h"
class DrawObj : public Button
{
private:
	string type;
	string path;
	bool isMove;
	EditorObjs data;
	map<string,Item> items;
public:
	DrawObj(UiMgr* uimgr);
	~DrawObj();

	virtual void Update(float dt);
	void SetType(string type) { this->type = type; }
	void SetPath(string path) { this->path = path; }
	void SetMove(bool move);
	string GetPath() { return path; }
	string GetType() { return type; }
	EditorObjs GetData()  { return data; }
	void SetData(EditorObjs d) { data = d; }
	void SetItem(map<string, Item> items) { this->items = items; }
	map<string, Item>& GetItem() { return items; }
	virtual void Release();
};

