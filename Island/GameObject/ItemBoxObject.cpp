#include "ItemBoxObject.h"
#include "../Framework/InputMgr.h"
#include "../Ui/GameSceneUiMgr.h"
#include "../Scens/GameScene.h"
#include "../Scens/SceneManager.h"
#include "../GameObject/Player.h"
#include "../Ui/InventoryBox.h"
#include "../Ui/Inventory.h"

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
		//((GameScene*)SCENE_MGR->GetCurrScene())->GetPlayer()->GetItem(&obj_items);;

		auto scene = ((GameScene*)SCENE_MGR->GetCurrScene());
		auto player = scene->GetPlayer();
		player->GetInventory()->SetRightInven(invenBox);
		player->GetInventory()->SetActive(true);
		player->GetInventory()->GetPlayerInven()->SetPair(invenBox);
		invenBox->SetActive(true);
	}
}

void ItemBoxObject::Draw(RenderWindow& window)
{
	HitBoxObject::Draw(window);
}

void ItemBoxObject::SetItems(map<string, Item> items)
{
	auto scene = ((GameScene*)SCENE_MGR->GetCurrScene());
	auto player = scene->GetPlayer();
	invenBox = new InventoryBox(scene->GetUiMgr(), player->GetInventory(), Vector2i{ 1248,252 });
	invenBox->SetName("RightInventory");
	invenBox->Init();
	invenBox->SetPair(player->GetInventory()->GetPlayerInven());
	cout << invenBox->GetPair()->GetName() << endl;
	obj_items = items;
	for (auto item : obj_items)
	{
		invenBox->AddItem(item.second.type);
	}
	this->obj_items = items;
	
}
