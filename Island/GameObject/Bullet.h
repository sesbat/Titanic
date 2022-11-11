#pragma once
#include "SpriteObject.h"

class Scene;
class HitBox;

class Bullet
	:public SpriteObject
{
private:
	Scene* scene;
	HitBox* hitbox;

	Vector2f dir;
	float speed;
	float range;

	//vector<Object*>* hitObjs;

public:
	Bullet();
	~Bullet();

	virtual void Init();
	//virtual void SetHitObjs(vector<Object*>* objs);
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	void Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range);

	void SetHitbox();
	bool EqualFloat(Vector2f a, Vector2f b);
	bool EqualFloat(float a, float b);
};

