#pragma once
#include "HitBoxObject.h"

class Bullet
	:public HitBoxObject
{
private:
	Vector2f dir;
	float speed;
	float range;
	vector<Object*>* hitObjs;

public:
	Bullet();
	~Bullet();

	virtual void Init();
	virtual void SetHitObjs(vector<Object*>* objs);
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	void Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range);
};

