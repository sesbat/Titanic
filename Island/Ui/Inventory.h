#pragma once
#include "Button.h"
class UiMgr;
class Inventory : public Button
{
private:
	const Vector2i startPos1 = Vector2i{163,252};
	const Vector2i startPos2 = Vector2i{ 1248,252 };
	const int padding = 5;

	const int width = 8;
	const int height = 11;

	vector<vector<int>> itemPos;

public:
	Inventory(UiMgr* mgr);
	virtual ~Inventory();
	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
};

