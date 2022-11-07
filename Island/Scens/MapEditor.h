#pragma once
#define WIDTHCNT 64
#define HEIGHTCNT 32
#include "../Scens/Scene.h"

class Button;
class UiMgr;
class MapEditor : public Scene
{
private:
	map<int, vector<Button*>> greeds;
	map<int, map<int,Button*>> greedObjs;
	Vector2f initMousePos;
	bool isMove;

public:
	MapEditor();
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual ~MapEditor();
};

