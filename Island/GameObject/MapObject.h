#pragma once
#include "../../GameObject/SpriteObject.h"

class HitBox;
class MapObject : public SpriteObject
{
private:
	vector<HitBox*> hitboxs;
	HitBox* bottom;
	bool isHitBox;
public:
	MapObject();
	virtual ~MapObject();
	void Init();
	void Update(float dt);
	void Draw(RenderWindow& window);
	void SetHitBox(string path);
};

