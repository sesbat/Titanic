#include "HitBox.h"
#include "../Framework/Utils.h"

HitBox::HitBox()
{
}

HitBox::~HitBox()
{
}

void HitBox::SetActive(bool active)
{
	Object::SetActive(active);
}

bool HitBox::GetActive() const
{
	return Object::GetActive();
}

void HitBox::Init()
{
}

void HitBox::Release()
{
}

void HitBox::Reset()
{
}

void HitBox::Update(float dt)
{
}

void HitBox::Draw(RenderWindow& window)
{
	if ( enabled )
	{
		window.draw(hitbox);
	}
}

void HitBox::SetInitPos(const Vector2f pos)
{
	initPos = pos;
}

void HitBox::SetPos(const Vector2f& pos)
{
	hitbox.setPosition(initPos + pos);
}

const Vector2f& HitBox::GetPos() const
{
	return Object::GetPos();
}

void HitBox::Translate(const Vector2f delta)
{
}

void HitBox::SetHitbox(const FloatRect rect)
{
	hitBoxRect = rect;
	hitbox.setSize({ rect.width, rect.height });
	hitbox.setFillColor(Color::Red);
	hitbox.setOutlineColor(Color::Black);
	hitbox.setOutlineThickness(2.f);
}

void HitBox::SetFillColor(Color color)
{
	hitbox.setFillColor(color);
}

const RectangleShape& HitBox::GetHitbox() const
{
	return hitbox;
}

//void HitBox::SetDevMode(bool dev)
//{
//	Object::SetDevMode(dev);
//}
