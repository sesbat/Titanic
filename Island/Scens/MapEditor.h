#pragma once
#define WIDTHCNT 64
#define HEIGHTCNT 32
#include "../Scens/Scene.h"

class Button;
class UiMgr;
class DrawObj;
class MapEditor : public Scene
{
private:

	map<int, vector<Button*>> greeds;
	map<LayerType,map<int, map<int, DrawObj*>>> greedObjs; //Tile, Object, Enermy
	vector<ObjectData> saveObjs;
	Vector2f initMousePos;
	bool isMove;
	string type;

	LayerType nowType;
	DrawObj* player;
	Vector2i playerPos;

public:
	MapEditor();
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual ~MapEditor();
	void SetType(string t);

	void Save();
	void Load();
};

