#pragma once
#include "ItemBoxObject.h"
class SupplyBox : public ItemBoxObject
{
private:
	SupplyBox(const ItemBoxObject& ref) {}
	SupplyBox& operator=(const ItemBoxObject& ref) {}
	SupplyBox(ItemBoxObject& ref) {}
	SupplyBox& operator=(ItemBoxObject& ref) {}
	VertexArray lines;

public:
	SupplyBox();
	virtual ~SupplyBox();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetSupplyItems(vector<InvenInfo> items);

	Vector2i GetRandSpawn();
	void SetRandPos();
};

