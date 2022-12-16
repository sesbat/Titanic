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
	Button* add10Cnt;
	Button* min10Cnt;

	AddItem(UiMgr* mgr);
	~AddItem();
	void SetPosition(Vector2f pos);
	void Update(float dt);
	void Draw(RenderWindow& window);

private:
	AddItem(const AddItem& ref)  {}
	AddItem& operator=(const AddItem& ref) {}
	AddItem(AddItem& ref) {}
	AddItem& operator=(AddItem& ref) {}

};

class AddItemBox : public Button
{
private:
	AddItemBox(const AddItemBox& ref) : Button(nullptr) {}
	AddItemBox& operator=(const AddItemBox& ref) {}
	AddItemBox(AddItemBox& ref) : Button(nullptr) {}
	AddItemBox& operator=(AddItemBox& ref) {}
	map<string,AddItem*> allItems;
	map<string,Item>* nowObjectItems;
	int addItemIdx = 0;
	
public:
	AddItemBox(UiMgr* mgr);
	~AddItemBox();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Release();
	void SetItems(map<string,Item>& items);
};

