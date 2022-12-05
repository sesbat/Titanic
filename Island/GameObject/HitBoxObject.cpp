#include "HitBoxObject.h"
#include "HitBox.h"
#include "../../Framework/InputMgr.h"

#include <iostream>

HitBoxObject::HitBoxObject()
	:isHitBox(false), isHitPlayer(false)
{
}

void HitBoxObject::SetPos(Vector2f pos)
{
	SpriteObject::SetPos(pos);
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
}

void HitBoxObject::Translate(Vector2f dir)
{
	SpriteObject::Translate(dir);
	;
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
}

HitBoxObject::~HitBoxObject()
{
	Release();
}

void HitBoxObject::Init()
{
	SpriteObject::Init();
}

void HitBoxObject::Update(float dt)
{
	SpriteObject::Update(dt);
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

void HitBoxObject::SetBulletHitBox()
{
	//bottom hit box
	bottom->SetHitbox({ 0,0,10.f,10.f });
	bottom->SetFillColor(Color::Green);
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

void HitBoxObject::Release()
{
	for (auto& hit : hitboxs)
	{
		delete hit;
	}
	hitboxs.clear();
}
