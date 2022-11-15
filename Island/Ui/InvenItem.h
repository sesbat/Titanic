#pragma once
#include "MoveObject.h"
class UiMgr;
class TextObject;

class InvenItem : public MoveObject
{
private:
	string path;
	int maxCount;
	int count;
	int width;
	int height;
	Vector2i invenPos;
	Vector2i invenGreedPos;
public:
	InvenItem(UiMgr* mgr);
	virtual ~InvenItem();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	int GetCount() { return count; }
	void Set(int width, int height, Vector2i invenPos,Vector2i invenGreedPos, string path, int maxCount);
	void SetInvenPos(Vector2i invenPos, Vector2i invenGreedPos);
	Vector2i GetInvenPos() { return invenPos; }
	Vector2i GetGreedPos() { return invenGreedPos; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void AddCount(int n);

	
};

