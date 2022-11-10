#include "Bullet.h"

Bullet::Bullet()
	:dir(), speed(0.f), range(0.f)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	SetOrigin(Origins::MC);
	SpriteObject::Init();
}

void Bullet::Update(float dt)
{
	SpriteObject::Update(dt);

	range -= Utils::Magnitude(dir * dt * speed);
	if (range >= 0.f)
	{
		Translate(dir * dt * speed);
	}
	else
	{
		SetActive(false);
	}
}

void Bullet::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
}

void Bullet::Reset()
{
	dir = { 1, 0 };
	speed = 0.f;
	range = 0.f;
	sprite.setRotation(0.f);
	SetPos({ 0, 0 });

	SpriteObject::Reset();
}

void Bullet::Release()
{
	SpriteObject::Release();
}

void Bullet::Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPos(pos);
	SetActive(true);
	this->dir = dir;
	this->speed = speed;
	this->range = range;
}
