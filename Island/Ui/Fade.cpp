#include "Fade.h"

Fade::Fade() :
	isFadein(false), isFadeOut(false),
	isStarted(false), rect(RectangleShape()), alpha(0), goal(0)
{
};

void Fade::SetFadeOut()
{
	isFadeOut = true;
	alpha = 0; goal = 255;
	rect.setFillColor(Color(0, 0, 0, 0));
}

void Fade::SetFadeIn()
{
	isFadein = true;
	alpha = 255; goal = 0;
	rect.setFillColor(Color(0, 0, 0, 255));
}

void Fade::SetSkip()
{
	if(isFadein)
	{
		alpha = 0;
		rect.setFillColor(Color(0, 0, 0, alpha));
	}
	if (isFadeOut)
	{
		alpha = 255;
		rect.setFillColor(Color(0, 0, 0, alpha));
	}
}

void Fade::SetRect(const Vector2f& size)
{
	this->rect.setSize(size);
}

void Fade::Update(float dt)
{
	if (isFadein && (alpha > goal)) // black > screen
	{
		alpha -= 10*dt;
		rect.setFillColor(Color(0, 0, 0, alpha));
	}
	else if (isFadeOut && (alpha < goal)) // screen > black
	{
		alpha += 10 * dt;
		rect.setFillColor(Color(0, 0, 0, alpha));
	}
}

void Fade::Draw(RenderWindow& window)
{
	window.draw(rect);
}

inline bool Fade::DoFade(float dt)
{
	//if (isFadein && (alpha > goal)) // black > screen
	//{
	//    alpha-= dt;
	//    rect.setFillColor(Color(0, 0, 0, alpha));
	//    return true;
	//}
	//else if (isFadeOut && (alpha < goal)) // screen > black
	//{
	//    alpha += dt;
	//    rect.setFillColor(Color(0, 0, 0, alpha));
	//    return true;
	//}
	//else { return false; }
}
