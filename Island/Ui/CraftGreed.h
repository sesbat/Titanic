#pragma once
#include "Button.h"

class UiMgr;
class Craft;
class CraftBox;
class InvenItem;
class CraftItem;
class CraftGreed : public Button
{
private:
	CraftGreed(const CraftGreed& ref) : Button(nullptr) {}
	CraftGreed& operator=(const CraftGreed& ref) {}
	CraftGreed(CraftGreed& ref) : Button(nullptr) {}
	CraftGreed& operator=(CraftGreed& ref) {}
	Vector2i invenPos;
	Craft* inven;
	CraftBox* invenBox;
	bool state;
	CraftItem* nowItem;
public:
	CraftGreed(UiMgr* mgr, Craft* inven, CraftBox* invenBox);
	~CraftGreed();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetState(bool s, CraftItem* itme);
	void SetInvenPos(int i, int j) { invenPos = { i,j }; }
	void SetInvenPos(Vector2i iPos) { invenPos = iPos; }
	Vector2i GetInvenPos() { return invenPos; }
	CraftItem* GetItem() { return nowItem; }

};

