#include "Bullet.h"
#include "../Scens/SceneManager.h"
#include "../GameObject/HitBox.h"
#include "../GameObject/HitBoxObject.h"
#include <iostream>

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
	HitBoxObject::Init();
}

void Bullet::SetHitObjs(vector<Object*>* objs)
{
	this->hitObjs = objs;
}

void Bullet::Update(float dt)
{
	HitBoxObject::Update(dt);
	isHitBox = false;
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
	HitBoxObject::Draw(window);
}

void Bullet::Reset()
{
	dir = { 1, 0 };
	speed = 0.f;
	range = 0.f;
	sprite.setRotation(0.f);
	SetPos({ 0, 0 });

	HitBoxObject::Reset();
}

void Bullet::Release()
{
	HitBoxObject::Release();
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
