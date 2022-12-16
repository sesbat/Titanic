#pragma once
#include "../3rd/Singleton.h"
#include <SFML/Graphics.hpp>

using namespace sf;
class Framework : public Singleton<Framework>
{
protected:
	RenderWindow window;
	Vector2i windowSize;
	Clock clock;
	Time deltaTime;
	float timeScale;

public:
	Framework();
	virtual ~Framework();

	float GetDT() const;
	float GetRealDT() const;

	RenderWindow& GetWindow() { return window; }
	const Vector2i& GetWindowSize()const;
	bool Init();
	bool Do();
};

#define FRAMEWORK (Framework::GetInstance())