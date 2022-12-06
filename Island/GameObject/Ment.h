#pragma once
#include "TextObject.h"
class Ment : public TextObject
{
private:
	Ment(const Ment& ref) {}
	Ment& operator=(const Ment& ref) {}
	Ment( Ment& ref) {}
	Ment& operator=( Ment& ref) {}
	float timer;
	float nowTime;
	bool always;
	
public:
	Ment();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Init();

	void SetTimer(float timer) { this->timer = timer; }
	void SetAlways(bool state) { always = state; }

	
};

