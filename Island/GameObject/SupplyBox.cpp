#include "SupplyBox.h"
#include "../Scens/SceneManager.h"
#include "../Scens/GameScene.h"
#include "../Ui/InventoryBox.h"
#include "Player.h"
#include "../Ui/Inventory.h"
#include "Ment.h"

SupplyBox::SupplyBox()
	:lines(LineStrip, 2)
{
	pickState = false; 
}

SupplyBox::~SupplyBox()
{
	Release();
}

void SupplyBox::Update(float dt)
{
	lines[1] = *playerPos;
	lines[0] = GetPos();
	ItemBoxObject::Update(dt);
}

void SupplyBox::Draw(RenderWindow& window)
{
	ItemBoxObject::Draw(window);
	window.draw(lines);
}

void SupplyBox::SetSupplyItems(vector<InvenInfo> items)
{
	auto scene = ((GameScene*)SCENE_MGR->GetCurrScene());
	auto player = scene->GetPlayer();
	invenBox = new InventoryBox(scene->GetUiMgr(), player->GetInventory(), Vector2i{ 1248,252 });
	invenBox->SetName("RightInventory");
	invenBox->Init();
	invenBox->SetPair(player->GetInventory()->GetPlayerInven());


	for (auto& data : items)
	{
		string type = data.Type;
		Vector2i invenPos = data.invenPos;
		Vector2i invenGreedPos = data.invenGreedPos;
		int cnt = data.cnt;
		string path = data.path;

		invenBox->AddItem(type, cnt, invenPos, invenGreedPos, path);
	}
}

Vector2i SupplyBox::GetRandSpawn()
{
	auto map_data = ((GameScene*)SCENE_MGR->GetCurrScene())->GetGreedObj();

	vector<Vector2i> checkPos;

	for (int i = 0; i < map_data.size(); i++)
	{
		for (int j = 0; j < map_data[i].size(); j++)
		{
			if (!map_data[i][j])
				checkPos.push_back(Vector2i{ i,j });
		}
	}

	int checkSize = checkPos.size();
	int getRendPos = Utils::RandomRange(0, checkSize);
	

	return checkPos[getRendPos];
}

void SupplyBox::SetRandPos()
{
	auto randPos = GetRandSpawn();

	SetPos(Vector2f(randPos.y * 60 + 30, randPos.x * 60 + 60));
	ment->SetPos(GetPos() + Vector2f{ 0,-50 });


}
