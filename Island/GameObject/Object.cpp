#include "Object.h"
#include "../Framework/Utils.h"
#include "../Framework/InputMgr.h"
#define DEBUG

#include <iostream>
Object::Object()
{
	hitDraw = true;
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
	std::cout << "Release" << std::endl;
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

void Object::AddHitBox(string path)
{
	std::cout << "ADD" << std::endl;
	auto data = FILE_MGR->GetHitBox(path);

	for (auto& d : data)
	{
		HitBox h;
		auto rect = new RectangleShape();
		rect->setFillColor(Color::Red);
		rect->setSize(d.size);
		h.shape = rect;
		h.initPos = d.pos;

		hitBoxs.push_back(h);
	}

	bottomHitBox = hitBoxs[0];
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
	if (enabled && hitDraw)
		for (auto& hitbox : hitBoxs)
		{
			window.draw(*hitbox.shape);
		}
}
