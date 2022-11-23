#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class SpriteObject : public Object
{
protected:
	Sprite sprite;
	bool isUi;
	bool viewIn; //ī�޶� �ȿ� ������Ʈ �ִ���
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
	virtual FloatRect GetGlobalBound();
	Sprite& GetSprite() { return sprite; };
	void SetUI(bool u) { isUi = u; }
	bool IsInView();
	void SetFlipX(bool flip);
	bool GetIsView() { return viewIn; }
	void SetFlipY(bool flip);
};

