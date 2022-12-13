#pragma once
#include "../Ui/Button.h"

class InvenItem;
class ToolTip : public Button
{
public:
	enum class ItemType
	{
		None = 0, Expendables = 1, Stuff, Wephon, Armor, Medical, Ammo, Another
	};

private:
	ItemType type;
	string itemName;
	vector<TextObject*> tooltipDatas;
	Vector2f itemPos;
	
public:
	ToolTip(UiMgr* uimgr);
	virtual void Release();
	virtual void init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual ~ToolTip();
	virtual void SetItemType(int i) { type = (ItemType)i; }
	virtual void SetItem(string name);

	void SetToolPos(Vector2f v);
	
	
};

