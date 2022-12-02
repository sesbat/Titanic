#pragma once
#include "../../GameObject/SpriteObject.h"

class HitBox;
class QuadTree;
class HitBoxObject : public SpriteObject
{
	friend class QuadTree;
protected:
	vector<HitBox*> hitboxs;
	HitBox* bottom;
	bool isHitBox;
	bool isHitPlayer;
	QuadTree* qt = nullptr;

	HitBoxObject(const HitBoxObject& ref) {}
	HitBoxObject& operator= (const HitBoxObject& ref) { return *this; }

public:
	HitBoxObject();
	virtual void SetPos(Vector2f pos);
	virtual void Translate(Vector2f dir);
	virtual ~HitBoxObject();
	void Init();
	void Update(float dt);
	void Draw(RenderWindow& window);
	void SetHitBox(string path);
	void SetBulletHitBox();
	const HitBox* GetBottom() { return bottom; } const
	const vector<HitBox*>& GetHitBoxs() { return hitboxs; }
	float GetBottomPos();
	void SetHitPlayer(bool h);
	bool GetHitColor() { return isHitPlayer; }

};

