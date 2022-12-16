#pragma once
#define WIDTHCNT 128
#define HEIGHTCNT 72
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

	DrawObj* now_exit;
	Vector2i exitPos;

	Vector2i view_start_pos;
	Vector2i veiw_end_pos;

public:
	MapEditor();
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Release() override;
	virtual ~MapEditor();
	void SetType(string t);

	void Save();
	void Load(string path);
	bool DrawBox(int x, int y);
	Button* GetGreee(int x, int y) { return greeds[x][y]; }
};

