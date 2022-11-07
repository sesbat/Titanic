#include "Button.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include <iostream>0

using namespace std;

Button::Button(UiMgr* mgr)
	: sprite(nullptr), text(nullptr), uimgr(mgr), bndType(BoundType::None), isClickColor(false)
{
	Init();
	sprite = new SpriteObject();
	text = new TextObject();
}

Button::~Button()
{
}

void Button::Init()
{
	UiObject::Init();
}

void Button::Reset()
{
	UiObject::Reset();
}

void Button::ColorClear()
{
	UiObject::ColorClear();
	if (isClickColor)
	{
		if (sprite->GetSprite().getTexture() != nullptr)
		{
			sprite->SetColor(Color::White);
		}
		if (text->GetText().getFont() != nullptr)
		{
			text->SetColor(Color::White);
		}
	}
}

void Button::Update(float dt)
{
	UiObject::Update(dt);
	if (IsDown())
	{
		if (isClickColor)
		{
			if (sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color(200, 200, 200, 255));
			}
			if (text->GetText().getFont() != nullptr)
			{
				text->SetColor(Color(200, 200, 200, 255));
			}
		}
	}
	if (IsUp() || IsExit())
	{
		if (isClickColor)
		{
			if (sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color::White);
			}
			if (text->GetText().getFont() != nullptr)
			{
				text->SetColor(Color::White);
			}
		}
	}
	if (IsDownRight())
	{
		if (isClickColor)
		{
			if (sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color(200, 200, 200, 255));
			}
			if (text->GetText().getFont() != nullptr)
			{
				text->SetColor(Color(200, 200, 200, 255));
			}
		}
	}
	if (IsUpRight() || IsExit())
	{
		if (isClickColor)
		{
			if (sprite->GetSprite().getTexture() != nullptr)
			{
				sprite->SetColor(Color::White);
			}
			if (text->GetText().getFont() != nullptr)
			{
				text->SetColor(Color::White);
			}
		}
	}
}

void Button::Draw(RenderWindow& window)
{
	UiObject::Draw(window);
	if (sprite->GetSprite().getTexture() != nullptr)
	{
		sprite->Draw(window);
	}
	if (text->GetText().getFont() != nullptr)
	{
		text->Draw(window);
	}
}

void Button::SetPos(Vector2f pos)
{
	UiObject::SetPos(pos);
	sprite->SetPos(pos);
	text->SetPos(pos);
	ReBound();
}

void Button::SetTexture(Texture& t, bool isBound)
{
	sprite->SetTexture(t);
	if (isBound)
	{
		SetBound(sprite->GetGlobalBound());
		bndType = BoundType::SPRIT;
	}
}

void Button::SetText(Font& font, int size, Color color, string str, bool isBound)
{
	text->SetText(font, size, color, str);
	if (isBound)
	{
		SetBound(text->GetGlobalBound());
		bndType = BoundType::TEXT;
	}
}

void Button::SetOrigin(Origins origin)
{
	if (sprite->GetSprite().getTexture() != nullptr)
	{
		sprite->SetOrigin(origin);
	}
	if (text->GetText().getFont() != nullptr)
	{
		text->SetOrigin(origin);
	}
	ReBound();
}

void Button::ReBound()
{
	switch (bndType)
	{
	case Button::BoundType::None:
		break;
	case Button::BoundType::SPRIT:
		SetBound(sprite->GetGlobalBound());
		break;
	case Button::BoundType::TEXT:
		SetBound(text->GetGlobalBound());
		break;
	default:
		break;
	}

}

std::ofstream& operator<<(std::ofstream& ofs, const Button& cur)
{
	switch (cur.GetState())
	{
	case UiState::None:
		cout << "None";
		break;
	case UiState::Enter:
		cout << "Enter";
		break;
	case UiState::Stay:
		cout << "Stay";
		break;
	case UiState::Down:
		cout << "Down";
		break;
	case UiState::Up:
		cout << "Up";
		break;
	case UiState::Exit:
		cout << "Exit";
		break;
	default:
		break;
	}
	return ofs;
}
