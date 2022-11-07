#pragma once
#define WIDTHCNT 64
#define HEIGHTCNT 32
#include "../Scens/Scene.h"

class Button;
class UiMgr;
class MapEditor : public Scene
{
private:
	map<int,vector<Button*>> greeds;
	UiMgr* uimgr;

public:
	MapEditor();
	virtual void Init();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void Draw(float dt);
	~MapEditor();
};

