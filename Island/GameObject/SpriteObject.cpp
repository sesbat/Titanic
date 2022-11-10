#include "SpriteObject.h"
#include "../Scens/SceneManager.h"

SpriteObject::SpriteObject()
	:isUi(false), viewIn(false)
{
}

SpriteObject::~SpriteObject()
{
	Release();
}

void SpriteObject::Init()
{
}

void SpriteObject::Update(float dt)
{
	Object::Update(dt);
}

void SpriteObject::Draw(RenderWindow& window)
{
	if(enabled /*&& IsInView()*/)
		window.draw(sprite);
	Object::Draw(window);
}

void SpriteObject::SetScale(Vector2f scale)
{
	sprite.setScale({ scale });
}
void SpriteObject::SetSize(Vector2f size)
{
	auto bound = sprite.getLocalBounds();
	sprite.setScale({ size.x / bound.width,size.y / bound.height });
}

void SpriteObject::SetPos(Vector2f pos)
{
	sprite.setPosition(pos);
	Object::SetPos(pos);
}

void SpriteObject::Translate(const Vector2f& delta)
{
	SetPos(position + delta);
}

void SpriteObject::SetTexture(Texture& tex)
{
	sprite.setTexture(tex,true);
}
void SpriteObject::SetColor(Color color)
{
	sprite.setColor(color);
}
Color SpriteObject::GetColor()
{
	return sprite.getColor();
}

void SpriteObject::SetOrigin(Origins origin)
{
	Utils::SetOrigin(sprite, origin);
}

FloatRect SpriteObject::GetGlobalBound()
{
	return sprite.getGlobalBounds();
}

bool SpriteObject::IsInView()
{
	if (isUi)
	{
		viewIn = true;
		return true;
	}

	auto& view = SCENE_MGR->GetCurrScene()->GetWorldView();
	auto& viewSize = view.getSize();
	auto& viewCenter = view.getCenter();
	auto bound = GetGlobalBound();

	if (((bound.left > viewCenter.x + viewSize.x / 2) || (bound.left + bound.width < viewCenter.x - viewSize.x / 2)) ||
		((bound.top > viewCenter.y + viewSize.y / 2) || (bound.top + bound.height < viewCenter.y - viewSize.y / 2)))
	{
		viewIn = false;
		return false;
	}
	viewIn = true;
	return true;
}
void SpriteObject::SetFlipX(bool flip)
{
	Vector2f scale = sprite.getScale();
	scale.x = flip ? -abs(scale.x) : abs(scale.x);
	sprite.setScale(scale);
}
void SpriteObject::SetFlipY(bool flip)
{
	Vector2f scale = sprite.getScale();
	scale.y = flip ? -abs(scale.y) : abs(scale.y);
	sprite.setScale(scale);
}

