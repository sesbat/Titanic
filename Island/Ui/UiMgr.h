#pragma once
#include "../GameObject/Object.h"
#include <list>

class UiObject;
class Scene;
class UiMgr : public Object
{
protected:
	map<int, vector<UiObject*>> uiObjList;
	Scene* parentScene;

	UiObject* nowEvObj;
	bool enabled;
public:
	UiMgr(Scene* scene);
	virtual ~UiMgr();

	virtual void Init();
	virtual void Release();

	virtual void SetPos(const Vector2f& pos);

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

