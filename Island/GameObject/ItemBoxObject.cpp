#include "ItemBoxObject.h"
#include "../Framework/InputMgr.h"
#include "../Ui/GameSceneUiMgr.h"
#include "../Scens/GameScene.h"
#include "../Scens/SceneManager.h"
#include "../GameObject/Player.h"
#include "../Ui/InventoryBox.h"
#include "../Ui/Inventory.h"
#include "../Framework/ResourceManager.h"
#include "Ment.h"
#include "Player.h"
#include "../Scens/Scene.h"
#include "../Scens/SceneManager.h"

ItemBoxObject::ItemBoxObject()
	:pickState(false)
{
}

ItemBoxObject::~ItemBoxObject()
{
	Release();
	//SCENE_MGR->GetCurrScene()->AddDeleteObject(1, ment);
}

void ItemBoxObject::Init()
{
	ment = new Ment();
	ment->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 40, Color::White, "[F]");
	ment->SetAlways(true);
	ment->SetOrigin(Origins::BC);
	ment->SetPos(GetPos() - Vector2f{ 0.f,50 });
	ment->SetActive(false);
	SCENE_MGR->GetCurrScene()->AddGameObject(ment, LayerType::Object, 1);
	HitBoxObject::Init();
}

void ItemBoxObject::SetPlayer(Player* player)
{
	this->player = player;
}
void ItemBoxObject::Reset()
{
	HitBoxObject::Reset();
}

void ItemBoxObject::Release()
{
	ment->SetColor(Color(0, 0, 0, 0));
	HitBoxObject::Release();
}

void ItemBoxObject::Update(float dt)
{
	HitBoxObject::Update(dt);

	if (!ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) < 50)
	{
		ment->SetActive(true);
	}
	else if (ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) >= 50)
	{
		ment->SetActive(false);
	}

	if (Utils::Distance(position, *playerPos) < 50 && InputMgr::GetKeyDown(Keyboard::F))
	{
		auto scene = ((GameScene*)SCENE_MGR->GetCurrScene());
		auto player = scene->GetPlayer();

		if (!player->GetIsMove())
			return;

		player->SetIsInven(false);
		player->GetInventory()->SetRightInven(invenBox);
		player->GetInventory()->SetActive(true);
		player->GetInventory()->GetPlayerInven()->SetPair(invenBox);
		player->SetRigthInvenBox(this);
		invenBox->SetActive(true);
		player->SetMove(false);
		InputMgr::Clear();
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
	obj_items = items;
	for (auto& item : obj_items)
	{
		auto data = FILE_MGR->GetItemInfo(item.first);
		int i = item.second.count / data.maxCount;
		
		for (int n = 0; n < i; n++)
		{
			invenBox->AddItem(item.second.type, data.maxCount);
		}
		invenBox->AddItem(item.second.type, item.second.count % data.maxCount);
	}
	this->obj_items = items;
	
}
