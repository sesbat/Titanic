#pragma once
#include "UiObject.h"
#include "../../Framework/Utils.h"

class SpriteObject;
class TextObject;
class UiMgr;

class Button : public UiObject
{

public:
	enum class BoundType
	{
		None = -1, SPRIT, TEXT
	};
protected:
	SpriteObject* sprite;
	TextObject* text;
	UiMgr* uimgr;
	BoundType bndType;
	bool isClickColor;

public:
	Button(UiMgr* mgr);
	virtual ~Button();
	virtual void Init();
	virtual void Reset();
	virtual void ColorClear();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetPos(Vector2f pos);
	virtual void SetClkColor(bool set) { isClickColor = set; }
	virtual void Release();
	void SetTexture(Texture& t, bool isBound);
	void SetText(Font& font, int size, Color color, string str, bool isBound);

	void SetOrigin(Origins origin);
	void ReBound();

	SpriteObject* GetSpriteObj() { return sprite; }
	TextObject* GetTextObj() { return text; }
};

