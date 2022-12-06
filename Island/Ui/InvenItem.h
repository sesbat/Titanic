#pragma once
#include "MoveObject.h"
class UiMgr;
class TextObject;

class InvenItem : public MoveObject
{
private:
	InvenItem(const InvenItem& ref) : MoveObject(nullptr) {}
	InvenItem& operator=(const InvenItem& ref) {}
	InvenItem(InvenItem& ref) : MoveObject(nullptr) {}
	InvenItem& operator=(InvenItem& ref) {}
	string path;
	int maxCount;
	int count;
	int width;
	int height;
	int price;
	Vector2i invenPos;
	Vector2i invenGreedPos;
	

public:
	InvenItem(UiMgr* mgr);
	virtual ~InvenItem();
	virtual void Release();
	virtual void Init();
	virtual void Update(float dt);
	void ButtonUpdate(float dt);
	virtual void Draw(RenderWindow& window);

	int GetCount() { return count; }
	int GetMaxCount() { return maxCount; }
	int GetPrice() { return price; }
	void Set(int width, int height, Vector2i invenPos,Vector2i invenGreedPos, string path, int maxCount, int price);
	void SetInvenPos(Vector2i invenPos, Vector2i invenGreedPos);
	void SetInvenPos(Vector2f invenGreedPos);
	Vector2i GetInvenPos() { return invenPos; }
	Vector2i GetGreedPos() { return invenGreedPos; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	string GetPath() { return path; }
	void AddCount(int n);

	void SetCount(int n);

	
};

