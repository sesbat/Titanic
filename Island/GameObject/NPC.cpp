#include "NPC.h"
#include "../Framework/Utils.h"
#include "Player.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Framework/ResourceManager.h"
#include "../Ui/Button.h"
#include "../Framework/Framework.h"
#include "../Ui/InventoryBox.h"
#include "../Scens/SceneManager.h"
#include "../Scens/Scene.h"
#include "../Ui/Inventory.h"

NPC::NPC()
	:isShowMap(false)
{
}

NPC::~NPC()
{
}

void NPC::SetPlayer(Player* player)
{
	this->player = player;
}

void NPC::SetNPCType(NPCType type)
{
	this->type = type;
}

void NPC::Init()
{
	scene = SCENE_MGR->GetCurrScene();
	shop = new InventoryBox(scene->GetUiMgr(),player->GetInventory(), Vector2i{ 1248,252 });
	shop->Init();
	NPCInven = player->GetInventory();
	NPCInven->SetRightInven(shop);

	SpriteObject::Init();
}

void NPC::Release()
{
	SpriteObject::Release();
}

void NPC::Update(float dt)
{
	SpriteObject::Update(dt);
	//cout << GetPos().x<<" "<< GetPos().y << endl;
	if (Utils::Distance(GetPos(), player->GetPos()) < 100 &&
		InputMgr::GetKeyDown(Keyboard::F))
	{
		switch (type)
		{
		case NPCType::Start:
			isShowMap = !isShowMap;
			break;
		case NPCType::Shop:
			player->GetInventory()->SetActive(!(player->GetInventory()->GetActive()));
			if (!player->GetInventory()->GetActive())
				player->GetInventory()->ClearInven();
			break;
		case NPCType::Count:
			break;
		default:
			break;
		}

	}
}

void NPC::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
}
