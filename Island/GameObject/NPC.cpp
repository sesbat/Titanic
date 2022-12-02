#include "NPC.h"
#include "../Framework/Utils.h"
#include "Player.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Framework/ResourceManager.h"
#include "../Ui/Button.h"
#include "../Framework/Framework.h"

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

void NPC::Init()
{
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
		isShowMap = !isShowMap;
	}
}

void NPC::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
}
