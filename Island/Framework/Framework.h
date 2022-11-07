#pragma once
#include "../3rd/Singleton.h"
#include <SFML/Graphics.hpp>

using namespace sf;
// Framework�� ���� ����� ���Ӹ��� ��� �ް� ��
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
	bool Init();
	bool Do();
};

#define FRAMEWORK (Framework::GetInstance())