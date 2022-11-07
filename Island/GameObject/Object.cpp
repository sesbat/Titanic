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

void Object::AddHitBox(const HitBoxInfo& hit, Vector2f pos)
{
	for (auto& ht : hit.rectangls)
	{
		AddHitBox(RectangleShape(ht.size), ht.pos);
	}
	for (auto& ht : hit.circles)
	{
		AddHitBox(CircleShape(ht.rad), ht.pos);
	}
	for (auto& ht : hit.points)
	{
		AddHitBox(ConvexShape(), ht.points, ht.pos);
	}
}

void Object::AddHitBox(const CookieHitBox& hit, Vector2f pos)
{
	for (auto& ht : hit.hitBox.rectangls)
	{
		AddHitBox(RectangleShape(ht.size), ht.pos);
	}
	for (auto& ht : hit.hitBox.circles)
	{
		AddHitBox(CircleShape(ht.rad), ht.pos);
	}
	for (auto& ht : hit.hitBox.points)
	{
		AddHitBox(ConvexShape(),ht.points, ht.pos);
	}

	AddHitBox(RectangleShape(hit.bottom.size), hit.bottom.pos, true);
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
void Object::AddHitBox(CircleShape hitbox, Vector2f pos, bool isBottom)
{
	HitBox hit = HitBox();
	hit.initPos = pos;
	hit.shape = new CircleShape(hitbox);
	Utils::SetOrigin(*hit.shape, Origins::TL);
	hit.shape->setFillColor(Color::Red);
	hit.SetPos(position);

	hitBoxs.push_back(hit);
	if (isBottom)
		bottomHitBox = hit;
}
void Object::AddHitBox(ConvexShape hitbox, vector<Vector2f> points, Vector2f pos, bool isBottom)
{
	HitBox hit = HitBox();

	hitbox.setPointCount(points.size());
	int idx = 0;
	for (auto& point : points)
	{
		hitbox.setPoint(idx, point);
		idx++;
	}

	hit.shape = new ConvexShape(hitbox);

	hit.SetPos(position);

	Utils::SetOrigin(*hit.shape, Origins::TL);


	hit.shape->setFillColor(Color(255,0,0,120));
	hit.initPos = pos;

	hitBoxs.push_back(hit);
	if (isBottom)
		bottomHitBox = hit;
}

void Object::AddHitBox(Shape*  hitbox, Vector2f initPos)
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
