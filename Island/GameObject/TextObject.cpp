#include "TextObject.h"

TextObject::TextObject()
{
}

void TextObject::Init()
{
}

void TextObject::Update(float dt)
{
	Object::Update(dt);
}

void TextObject::Draw(RenderWindow& window)
{
	if (!enabled)
	{
		return;
	}
	window.draw(text);
	Object::Draw(window);
}

void TextObject::SetText(Font& font, int size, Color color, wstring str)
{
	text.setFont(font);
	text.setCharacterSize(size);
	text.setString(str);
	text.setFillColor(color);
}

void TextObject::SetText(Font& font, int size, Color color, string str)
{
	text.setFont(font);
	text.setCharacterSize(size);
	text.setString(str);
	text.setFillColor(color);
}

void TextObject::SetTextLine(Color color, float size)
{
	text.setOutlineColor(color);
	text.setOutlineThickness(size);
}

void TextObject::SetPos(Vector2f pos)
{
	text.setPosition(pos);
	Object::SetPos(pos);
}

void TextObject::SetOrigin(Origins origin)
{
	Utils::SetOrigin(text, origin);
}

void TextObject::SetString(string str)
{
	text.setString(str);
}

void TextObject::SetColor(Color color)
{
	text.setFillColor(color);
}
