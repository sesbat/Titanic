#pragma once
#include "SpriteObject.h"

class Bullet
	:public SpriteObject
{
private:
	Vector2f dir;
	float speed;
	float range;

public:
	Bullet();
	~Bullet();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	void Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range);
};

