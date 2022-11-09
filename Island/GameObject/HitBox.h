#pragma once
#include "Object.h"

class HitBox2 : public Object
{
protected:
	RectangleShape hitbox;
	FloatRect hitBoxRect;

public:
	HitBox2();
	virtual ~HitBox2();

	virtual void SetActive(bool active);
	virtual bool GetActive() const;

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;
	virtual void Translate(const Vector2f delta);

	void SetHitbox(const FloatRect rect);
	void SetFillColor(Color color);
	const RectangleShape& GetHitbox() const;
	FloatRect& GetHitBoxRect() { return hitBoxRect; }


	//virtual	void SetDevMode(bool dev);
};

