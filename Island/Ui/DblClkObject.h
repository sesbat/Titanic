#pragma once
#include "Button.h"

class UiMgr;
class DblClkObject : public Button
{
private:
	float timer;
	float dkTime;
	bool dkStart;

	float timerR;
	float dkTimeR;
	bool dkRStart;

	bool isDK;
	bool isDKR;
public:
	DblClkObject(UiMgr* uimgr);
	virtual ~DblClkObject();
	virtual void Update(float dt);
	virtual void Reset();
	virtual void Init();
	virtual void Release();

	virtual bool IsDoubleClick() { return isDK; }
	virtual bool IsDoubleClickRight() { return isDKR; }
};

