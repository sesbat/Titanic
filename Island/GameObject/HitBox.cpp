#include "HitBox.h"
#include "../Framework/Utils.h"

HitBox2::HitBox2()
{
}

HitBox2::~HitBox2()
{
}

void HitBox2::SetActive(bool active)
{
	Object::SetActive(active);
}

bool HitBox2::GetActive() const
{
	return Object::GetActive();
}

void HitBox2::Init()
{
}

void HitBox2::Release()
{
}

void HitBox2::Reset()
{
}

void HitBox2::Update(float dt)
{
}

void HitBox2::Draw(RenderWindow& window)
{
	if ( enabled )
	{
		window.draw(hitbox);
	}
}

void HitBox2::SetInitPos(const Vector2f pos)
{
	initPos = pos;
}

void HitBox2::SetPos(const Vector2f& pos)
{
	hitbox.setPosition(initPos + pos);
}

const Vector2f& HitBox2::GetPos() const
{
	return Object::GetPos();
}

void HitBox2::Translate(const Vector2f delta)
{
}

void HitBox2::SetHitbox(const FloatRect rect)
{
	hitBoxRect = rect;
	hitbox.setSize({ rect.width, rect.height });
	hitbox.setFillColor(Color::Red);
	hitbox.setOutlineColor(Color::Black);
	hitbox.setOutlineThickness(2.f);
}

void HitBox2::SetFillColor(Color color)
{
	hitbox.setFillColor(color);
}

const RectangleShape& HitBox2::GetHitbox() const
{
	return hitbox;
}

//void HitBox::SetDevMode(bool dev)
//{
//	Object::SetDevMode(dev);
//}
