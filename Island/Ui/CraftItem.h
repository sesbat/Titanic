#pragma once
#include "Button.h"
class UiMgr;
class TextObject;

class CraftItem : public Button
{
private:
	CraftItem(const CraftItem& ref) : Button(nullptr) {}
	CraftItem& operator=(const CraftItem& ref) {}
	CraftItem(CraftItem& ref) : Button(nullptr) {}
	CraftItem& operator=(CraftItem& ref) {}
	string path;
	int maxCount;
	int count;
	int width;
	int height;
	Vector2i invenPos;
	Vector2i invenGreedPos;

	int material;
public:
	CraftItem(UiMgr* mgr);
	virtual ~CraftItem();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	int GetCount() { return count; }
	int GetMaxCount() { return maxCount; }
	void Set(int width, int height, Vector2i invenPos, Vector2i invenGreedPos, string path, int maxCount);
	void SetInvenPos(Vector2i invenPos, Vector2i invenGreedPos);
	void SetInvenPos(Vector2f invenGreedPos);
	Vector2i GetInvenPos() { return invenPos; }
	Vector2i GetGreedPos() { return invenGreedPos; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	int GetMetarial() { return material; }
	void AddCount(int n);


};

