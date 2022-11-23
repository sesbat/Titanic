#pragma once
#include "Button.h"

class SpriteObject;
class TextObject;
struct Item;
struct AddItem
{
	SpriteObject* sprite;
	Item item;
	TextObject* text;
	Button* addCnt;
	Button* minCnt;

	AddItem(UiMgr* mgr);
	void SetPosition(Vector2f pos);
	void Update(float dt);
	void Draw(RenderWindow& window);
};

class AddItemBox : public Button
{
private:
	map<string,AddItem*> allItems;
	map<string,Item>* nowObjectItems;
	
public:
	AddItemBox(UiMgr* mgr);
	~AddItemBox();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetItems(map<string,Item>& items);
};

