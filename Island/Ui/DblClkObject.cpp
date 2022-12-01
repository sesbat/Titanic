#include "DblClkObject.h"

DblClkObject::DblClkObject(UiMgr* uimgr)
	:Button(uimgr), isDK(false), isDKR(false), timer(0.f), timerR(0.f), dkStart(false), dkRStart(false)
{
	Init();
}

DblClkObject::~DblClkObject()
{
}

void DblClkObject::Update(float dt)
{
	Button::Update(dt);
	isDK = false;
	if (IsClick() && !dkStart)
	{
		dkStart = true;
		timer = 0.f;
		return;
	}
	if (dkStart)
	{
		timer += dt;

		if (timer > 0.8f)
		{
			timer = 0.f;
			dkStart = false;
		}
		else if(IsClick())
		{
			isDK = true;

			timer = 0.f;
			dkStart = false;
		}
	}

	isDKR = false;
	if (IsClickRight() && !dkRStart)
	{
		dkRStart = true;
		timerR = 0.f;
		return;
	}
	if (dkRStart)
	{
		timerR += dt;

		if (timerR > 0.8f)
		{
			timerR = 0.f;
			dkRStart = false;
		}
		else if (IsClickRight())
		{
			isDKR = true;

			timerR = 0.f;
			dkRStart = false;
		}
	}
}

void DblClkObject::Reset()
{
	isDK = isDKR = false;
	timer = timerR = 0.f;
	dkStart = dkRStart = false;
	Button::Reset();
}

void DblClkObject::Init()
{
	Reset();
	Button::Init();
}
