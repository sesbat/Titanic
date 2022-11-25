#pragma once
#include "Button.h"

class UiMgr;
class Craft;
class CraftBox;
class InvenItem;
class CraftGreed : public Button
{
private:
	Vector2i invenPos;
	Craft* inven;
	CraftBox* invenBox;
	bool state;
	InvenItem* nowItem;
public:
	CraftGreed(UiMgr* mgr, Craft* inven, CraftBox* invenBox);
	~CraftGreed();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetState(bool s, InvenItem* itme);
	void SetInvenPos(int i, int j) { invenPos = { i,j }; }
	void SetInvenPos(Vector2i iPos) { invenPos = iPos; }
	Vector2i GetInvenPos() { return invenPos; }
	InvenItem* GetItem() { return nowItem; }

};

