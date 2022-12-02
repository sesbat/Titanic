#include "ItemBoxObject.h"
#include "../Framework/InputMgr.h"
#include "../Ui/GameSceneUiMgr.h"
#include "../Scens/GameScene.h"
#include "../Scens/SceneManager.h"

ItemBoxObject::ItemBoxObject()
	:pickState(false)
{
}

ItemBoxObject::~ItemBoxObject()
{
}

void ItemBoxObject::Init()
{
	HitBoxObject::Init();
}

void ItemBoxObject::Reset()
{
	HitBoxObject::Reset();
}

void ItemBoxObject::Release()
{
	HitBoxObject::Release();
}

void ItemBoxObject::Update(float dt)
{
	HitBoxObject::Update(dt);

	if (Utils::Distance(position, *playerPos) < 100 && InputMgr::GetKeyDown(Keyboard::F))
	{
		((GameScene*)SCENE_MGR->GetCurrScene())->GetItem(&items);;
	}
}

void ItemBoxObject::Draw(RenderWindow& window)
{
	HitBoxObject::Draw(window);
}

void ItemBoxObject::SetItems(map<string, Item> items)
{
	this->items = items;
}
