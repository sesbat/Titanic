#include "Bullet.h"
#include "HitBox.h"
#include "HitBoxObject.h"
#include "../Scens/SceneManager.h"
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
	SetOrigin(Origins::MR);
	SpriteObject::Init();
	scene = SCENE_MGR->GetCurrScene();
}



void Bullet::Update(float dt)
{
	if (GetActive())
	{
		SpriteObject::Update(dt);
		//isHitBox = false;
		range -= Utils::Magnitude(dir * dt * speed);
		if (range >= 0.f)
		{
			Translate(dir * dt * speed);

		}
		else
		{
			SetActive(false);

		}

		if (GetActive())
		{

		}
		// collision
		//auto obj = scene->GetObjList();
		//for (auto& objects : obj[LayerType::Object][0])
		//{
		//	auto hit = ((HitBoxObject*)objects)->GetBottom();
		//	if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
		//		continue;

		//	if (objects->GetName() == "TREE" ||
		//		objects->GetName() == "STONE" ||
		//		objects->GetName() == "ENEMY")
		//	{
		//		if (objects->GetPos() == GetPos())
		//		{
		//			cout << "hit" << endl;
		//			SetActive(false);
		//			//hitbox->SetActive(false);
		//			//hitbox->Release();
		//		}
		//	}
		//}
	}
	
}

void Bullet::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
	//hitbox->Draw(window);
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

void Bullet::SetHitbox()
{
	hitbox = new HitBox();
	hitbox->SetHitbox(FloatRect(0.f, 0.f, 10.f, 10.f));
}

bool Bullet::EqualFloat(Vector2f a, Vector2f b)
{
	return (EqualFloat(a.x, b.x) && EqualFloat(a.y, b.y));
}

bool Bullet::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}
