#pragma once
#include "Button.h"
class UiMgr;
class TextObject;

class InvenItem : public Button
{
private:
	int count;
	TextObject* countTxt;
	int width;
	int height;
	Vector2i invenPos;
public:
	InvenItem(UiMgr* mgr);
	virtual ~InvenItem();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

