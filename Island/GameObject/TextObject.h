#pragma once
#include "Object.h"
#include "../Framework/Utils.h"

class TextObject : public Object
{
protected:
	Text text;
public:
	TextObject();
	virtual ~TextObject();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void SetText(Font& font, int size, Color color, string str);
	void SetTextLine(Color color, float size);
	void SetPos(Vector2f pos);
	void SetOrigin(Origins origin);
	void SetString(string str);
	void SetColor(Color color);
	Text& GetText() { return text; }
	string GetString() { return text.getString(); }
	FloatRect GetGlobalBound() { return text.getGlobalBounds(); }

};


