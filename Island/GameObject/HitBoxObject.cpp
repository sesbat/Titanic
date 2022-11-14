#include "HitBoxObject.h"
#include "HitBox.h"
#include "../../Framework/InputMgr.h"

#include <iostream>

HitBoxObject::HitBoxObject()
	:isHitBox(true), isHitPlayer(false)
{
}

HitBoxObject::~HitBoxObject()
{
	for (auto& hit : hitboxs)
	{
		delete hit;
	}
}

void HitBoxObject::Init()
{
}

void HitBoxObject::Update(float dt)
{
	SpriteObject::Update(dt);
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());

	}
	if (InputMgr::GetKeyDown(Keyboard::F1))
	{
		isHitBox = !isHitBox;
	}
}

void HitBoxObject::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}
}

void HitBoxObject::SetHitBox(string path)
{
	if (hitboxs.size() != 0)
		return;
	auto hitData = FILE_MGR->GetHitBox(path);

	if (hitData.size() == 0)
	{
		bottom = nullptr;
		return;
	}

	for (auto& hit : hitData)
	{
		auto hitbox = new HitBox();
		hitbox->SetHitbox(FloatRect(hit.pos.x, hit.pos.y, hit.size.x, hit.size.y));
		hitbox->SetInitPos(hit.pos);
		hitbox->SetPos(GetPos());
		hitbox->SetActive(true);
		hitboxs.push_back(hitbox);
	}

	//enemy bottom hit box
	bottom = hitboxs[0];
	bottom->SetFillColor(Color::Blue);

}

float HitBoxObject::GetBottomPos()
{
	auto rect = bottom->GetHitbox().getGlobalBounds();
	return rect.top + rect.height;
}

void HitBoxObject::SetHitPlayer(bool h)
{
	isHitPlayer = h;
	SetColor(h ? Color(255, 255, 255, 100): Color::White);
}
