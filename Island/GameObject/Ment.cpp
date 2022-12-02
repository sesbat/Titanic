#include "Ment.h"
#include "../Scens/SceneManager.h"
#include <iostream>

Ment::Ment()
	:timer(0.f), nowTime(0.f), always(true)
{
	
}

Ment::~Ment()
{
}

void Ment::Update(float dt)
{
	if (!enabled)
		return;
	if (!always)
	{
		nowTime += dt;

		if (nowTime >= timer)
		{
			SCENE_MGR->GetCurrScene()->AddDeleteObject(1, this);
			SetActive(false);

			//std::cout << "Add Delete" << std::endl;
		}
	}

	TextObject::Update(dt);
}

void Ment::Draw(RenderWindow& window)
{
	if (!enabled)
		return;

	TextObject::Draw(window);
}

void Ment::Init()
{
	TextObject::Init();
}
