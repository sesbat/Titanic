#include "MapObject.h"
#include "HitBox.h"
#include "../../Framework/InputMgr.h"

MapObject::MapObject()
	:isHitBox(true)
{
}

MapObject::~MapObject()
{
}

void MapObject::Init()
{
}

void MapObject::Update(float dt)
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

void MapObject::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
		bottom->Draw(window);
	}
}

void MapObject::SetHitBox(string path)
{
	auto hitData = FILE_MGR->GetHitBox(path);

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
