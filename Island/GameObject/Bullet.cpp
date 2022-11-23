#include "Bullet.h"
#include "Player.h"
#include "HitBox.h"
#include "HitBoxObject.h"
#include "Enemy.h"
#include "../Scens/SceneManager.h"
#include "../Framework/InputMgr.h"
#include <iostream>

Bullet::Bullet()
	:dir(), speed(0.f), range(0.f), showLine(false)
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
		
		//position
		range -= Utils::Magnitude(dir * dt * speed);
		
		// collision
		Collision();
		if (GetActive())
		{
			if (range >= 0.f)
			{
				Translate(dir * dt * speed);	
				
				
			}
			else
			{
				SetActive(false);
			}
		}
		nextPos = GetPos() - dir * ((float)sprite.getLocalBounds().width);
	}
	if (InputMgr::GetKeyDown(Keyboard::P))
	{
		showLine = !showLine;
	}
}

void Bullet::Draw(RenderWindow& window)
{
	VertexArray lines(LineStrip, 2);
	lines[0].position = { startPos - dir * ((float)sprite.getLocalBounds().width) };
	lines[1].position = { nextPos };
	if (showLine)
	{
		window.draw(lines);
	}
	
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

void Bullet::Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range, bool isplayer)
{
	sprite.setRotation(Utils::Angle(dir));
	startPos = pos;
	nextPos = startPos - dir * ((float)sprite.getLocalBounds().width);
	SetPos(pos);
	this->isplayer = isplayer;
	SetActive(true);
	this->dir = dir;
	this->speed = speed;
	this->range = range;
}

bool Bullet::EqualFloat(Vector2f a, Vector2f b)
{
	return (EqualFloat(a.x, b.x) && EqualFloat(a.y, b.y));
}

bool Bullet::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

bool Bullet::LineRect(Vector2f bulletpos, Vector2f bulletPrevPos, RectangleShape hitObject)
{
	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	auto bounds = hitObject.getGlobalBounds();
	float rx = bounds.left;
	float ry = bounds.top;
	float rw = bounds.width;
	float rh = bounds.height;
	bool left = Lineline(bulletpos, bulletPrevPos, rx, ry, rx, ry + rh);
	bool right = Lineline(bulletpos, bulletPrevPos, rx + rw, ry, rx + rw, ry + rh);
	bool top = Lineline(bulletpos, bulletPrevPos, rx, ry, rx + rw, ry);
	bool bottom = Lineline(bulletpos, bulletPrevPos, rx, ry + rh, rx + rw, ry + rh);

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}
	return false;
}

bool Bullet::Lineline(Vector2f bulletpos, Vector2f bulletPrevPos, float x3, float y3, float x4, float y4)
{
	// calculate the direction of the lines
	float uA = ((x4 - x3) * (bulletpos.y - y3) - (y4 - y3) * (bulletpos.x - x3)) / 
		((y4 - y3) * (bulletPrevPos.x - bulletpos.x) - (x4 - x3) * (bulletPrevPos.y - bulletpos.y));
	float uB = ((bulletPrevPos.x - bulletpos.x) * (bulletpos.y - y3) - (bulletPrevPos.y - bulletpos.y) * (bulletpos.x - x3)) / 
		((y4 - y3) * (bulletPrevPos.x - bulletpos.x) - (x4 - x3) * (bulletPrevPos.y - bulletpos.y));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) 
	{
		return true;
	}
	return false;
}

void Bullet::Collision()
{
	auto obj = scene->GetObjList();
	for (auto& objects : obj[LayerType::Object][0])
	{
		auto hit = ((HitBoxObject*)objects)->GetBottom();
		//cout << hit->GetHitbox().getPosition().x << " " 
		//	<< hit->GetHitbox().getPosition().y << endl;

		if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
			continue;

		if (objects->GetName() == "TREE" ||
			objects->GetName() == "STONE" ||
			objects->GetName() == "BLOCK")
		{
			if (LineRect(
				startPos - dir * ((float)sprite.getLocalBounds().width),
				nextPos,
				hit->GetHitbox()))
			{
				speed = 0;
				SetActive(false);
				//cout << "hit" << endl;
				break;
			}
		}
		else if (isplayer && objects->GetName() == "ENEMY")
		{
			auto hb = ((HitBoxObject*)objects)->GetHitBoxs();
			for (auto it : hb)
			{
				for (Enemy* enemy : *enemies)
				{
					if (objects->GetId() == enemy->GetId())
					{
						if (LineRect(
							startPos - dir * ((float)sprite.getLocalBounds().width),
							nextPos,
							it->GetHitbox()))
						{
							//cout << "hit" << endl;
							enemy->SetHp(10);
							SetActive(false);
							break;
						}
						break;
					}
				}
			}
		}
		else if (!isplayer && objects->GetName() == "PLAYER")
		{
			auto hb = ((HitBoxObject*)objects)->GetHitBoxs();
			for (auto it : hb)
			{
				if (LineRect(
					startPos - dir * ((float)sprite.getLocalBounds().width),
					nextPos,
					it->GetHitbox()))
				{
					//cout << "player hit" << endl;
					player->SetHp(1);
					SetActive(false);
					break;
				}
			}
		}
	}
}
