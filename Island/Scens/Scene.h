#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <string>
#include "../Ui/UiMgr.h"

using namespace sf;
using namespace std;

class Object;
class HitBoxObject;
enum class Scenes
{
	None = -1, Menu,Ready,MapEditor,Count,GameScene, Tutorial
};

// �ٴ�, Ǯ, ������Ʈ + ��(Ư��), ���ʹ�
enum class LayerType
{
	None, Back, Tile, Object, Cursor, COUNT
};

class Scene
{
protected:
	Scenes type;

	map<LayerType, map<int, vector<Object*>>> objList;
	View worldView;
	View uiView;

	UiMgr* uiMgr;

	vector <Object*> deleteContainer;
	vector<Object*> moves;
	vector<Object*> another;
	vector<Object*> drawObjs;
	vector<Vector2f> radPos;
	vector<Object*> radObj;
	vector<HitBoxObject*> alphaObj;
	bool isGameScene;

	string sceneName;
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
	map<LayerType, map<int, vector<Object*>>> GetObjList() { return objList; }
	virtual void LayerSort();
	void SetSceneName(string name) { sceneName = name; }
	string GetSceneName() { return sceneName; }
	const vector<Vector2f>& GetRadPos() { return radPos; }
	const vector<Object*>& GetRadObj() { return radObj; }
	void AddDeleteObject(int idx,Object* obj);
};
bool sorting(Object* p1, Object* p2);
