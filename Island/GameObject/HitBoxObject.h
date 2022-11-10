#pragma once
#include "../../GameObject/SpriteObject.h"

class HitBox;
class HitBoxObject : public SpriteObject
{
protected:
	vector<HitBox*> hitboxs;
	HitBox* bottom;
	bool isHitBox;
public:
	HitBoxObject();
	virtual ~HitBoxObject();
	void Init();
	void Update(float dt);
	void Draw(RenderWindow& window);
	void SetHitBox(string path);
	HitBox* GetBottom() { return bottom; }
	const vector<HitBox*>& GetHitBoxs() { return hitboxs; }
};

