#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <string>
#include "../Ui/UiMgr.h"

using namespace sf;
using namespace std;

class Object;
enum class Scenes
{
	None = -1, Menu,Ready,MapEditor,Count,Dev2
};
// 바닥, 풀, 오브젝트 + 벽(특수), 에너미
enum class LayerType
{
	None, Back, Tile, Object
};

class Scene
{
protected:
	Scenes type;

	map<LayerType, map<int, vector<Object*>>> objList;
	View worldView;
	View uiView;

	UiMgr* uiMgr;

	vector<Object*> moves;
	vector<Object*> drawObjs;
	vector<Object*> hitTree;

	bool isMap;

public:
	Scene(Scenes type);
	virtual ~Scene();

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init() = 0;
	virtual void Reset();
	virtual void Release();

	View& GetWorldView() { return worldView; }
	View& GetUiView() { return uiView; }

	Vector2f ScreenToWorld(Vector2i screenPos);
	Vector2f ScreenToUiPosition(Vector2i screenPos);

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void AddGameObject(Object* obj, LayerType type, int num);
	Object* FindGameObj(string name);
	UiMgr* GetUiMgr() { return uiMgr; }
	virtual void LayerSort();
};

