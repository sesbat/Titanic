#pragma once
#include "Button.h"

class UiMgr;
class Inventory;
class InvenGreed : public Button
{
private:
	Vector2i invenPos;
	Inventory* inven;
	bool state;
public:
	InvenGreed(UiMgr* mgr, Inventory* inven);
	~InvenGreed();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetState(bool s);
	void SetInvenPos(int i, int j) { invenPos = { i,j }; }
	void SetInvenPos(Vector2i iPos) { invenPos = iPos; }
	Vector2i GetInvenPos() { return invenPos; }

};

