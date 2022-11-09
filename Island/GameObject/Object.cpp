#include "Object.h"
#include "../Framework/Utils.h"
#include "../Framework/InputMgr.h"
#define DEBUG

Object::Object()
{
	hitDraw = false;
	Init();
}

Object::~Object()
{
	Release();
}

bool Object::GetActive() const
{
	return enabled;
}

void Object::SetActive(bool active)
{
	enabled = active;
}

void Object::Init()
{
	enabled = true;
}

void Object::Release()
{
	for (auto& hitbox : hitBoxs)
	{
		if (hitbox.shape != nullptr)
			delete hitbox.shape;
	}
	hitBoxs.clear();
}

void Object::Reset()
{
}

void Object::SetPos(const Vector2f& pos)
{
	position = pos;
	for (auto& hit : hitBoxs)
	{
		hit.SetPos(position);
	}
}
void Object::Translate(const Vector2f& delta)
{
	SetPos(position + delta);
}

const Vector2f& Object::GetPos() const
{
	return position;
}

void Object::AddHitBox(vector<ns::RectangleInfo>& hit, Vector2f pos)
{
	for (auto& ht : hit)
	{
		AddHitBox(RectangleShape(ht.size), ht.pos);
	}
}

void Object::AddHitBox(RectangleShape hitbox, Vector2f pos, bool isBottom)
{
	HitBox hit = HitBox();
	hit.initPos = pos;
	hit.shape = new RectangleShape(hitbox);
	hit.shape->setFillColor(Color::Red);
	hit.SetPos(position);

	hitBoxs.push_back(hit);

	if (isBottom)
		bottomHitBox = hit;
	Utils::SetOrigin(*hit.shape, Origins::TL);
}

void Object::AddHitBox(Shape* hitbox, Vector2f initPos)
{
	HitBox hit = HitBox();

	hit.shape = hitbox;
	hit.initPos = initPos;
	hit.SetPos(position);
	hit.shape->setFillColor(Color(255, 0, 0, 120));
	hitBoxs.push_back(hit);
}

void Object::AddHitBox(HitBox hit)
{
	hit.SetPos(position);
	hit.shape->setFillColor(Color(255, 0, 0, 120));
	hitBoxs.push_back(hit);
}

const vector<HitBox>* Object::GetHitBox()
{
	return &hitBoxs;
}

const HitBox& Object::GetHitBoxBottom()
{
	return bottomHitBox;
}

void Object::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::F12))
	{
		hitDraw = false;
	}
	if (InputMgr::GetKeyDown(Keyboard::F11))
	{
		hitDraw = true;
	}
}

void Object::Draw(RenderWindow& window)
{
	if(enabled && hitDraw)
	for (auto& hitbox : hitBoxs)
	{
		window.draw(*hitbox.shape);
	}
}
