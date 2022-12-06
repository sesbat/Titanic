#include "Object.h"
#include "../Framework/Utils.h"
#include "../Framework/InputMgr.h"
#define DEBUG

#include <iostream>
Object::Object()
{
	Init();
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

void Object::Reset()
{
}

void Object::SetPos(const Vector2f& pos)
{
	position = pos;
}
void Object::Translate(const Vector2f& delta)
{
	SetPos(position + delta);
}

const Vector2f& Object::GetPos() const
{
	return position;
}

void Object::Update(float dt)
{
}

void Object::Draw(RenderWindow& window)
{
}
