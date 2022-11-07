#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class SpriteObject : public Object
{
protected:
	Sprite sprite;
public:
	SpriteObject();
	virtual ~SpriteObject();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetScale(Vector2f scale);
	void SetSize(Vector2f size);
	virtual void SetPos(Vector2f pos);

	virtual void Translate(const Vector2f& delta);

	void SetTexture(Texture& tex);
	void SetColor(Color color);
	Color GetColor();
	void SetOrigin(Origins origin);
	FloatRect GetGlobalBound();
	Sprite& GetSprite() { return sprite; };
};

