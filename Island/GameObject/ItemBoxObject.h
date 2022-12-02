#pragma once
#include "HitBoxObject.h"
class ItemBoxObject : public HitBoxObject
{
private:
	bool pickState;
	map<string, Item> items;
	Vector2f* playerPos;
public:
	ItemBoxObject();
	~ItemBoxObject();
	virtual void Init();
	virtual void Reset();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetItems(map<string, Item> items);
	void SetPlayerPos(Vector2f* pos) { playerPos = pos; }
};

