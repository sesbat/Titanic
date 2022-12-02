#pragma once
#include "TextObject.h"
class Ment : public TextObject
{
private:
	float timer;
	float nowTime;
	bool always;
	
public:
	Ment();
	virtual ~Ment();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Init();

	void SetTimer(float timer) { this->timer = timer; }
	void SetAlways(bool state) { always = state; }

	
};

