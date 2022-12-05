#include "NPC.h"
#include "../Framework/Utils.h"
#include "Player.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Framework.h"
#include "../Framework/ResourceManager.h"
#include "../Ui/Button.h"
#include "../Framework/Framework.h"
#include "../Scens/SceneManager.h"
#include "../Scens/Scene.h"
#include "Ment.h"

NPC::NPC()
	:isShowMap(false)
{
}

NPC::~NPC()
{
	Release();
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

	ment = new Ment();
	ment->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 40, Color::White, "[F]");
	ment->SetAlways(true);
	ment->SetOrigin(Origins::BC);
	ment->SetPos(GetPos() - Vector2f{ 0.f,GetGlobalBound().height + 10 });
	ment->SetActive(false);
	SCENE_MGR->GetCurrScene()->AddGameObject(ment, LayerType::Object, 1);

	SpriteObject::Init();
}

void NPC::Release()
{
	HitBoxObject::Release();
}

void NPC::Update(float dt)
{
	SpriteObject::Update(dt);

	if (!ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) < 100)
	{
		ment->SetActive(true);
	}
	else if(ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) >= 100)
	{
		ment->SetActive(false);
	}

	if (InputMgr::GetKeyDown(Keyboard::F) || InputMgr::GetKeyDown(Keyboard::Escape))
	{
		if (Utils::Distance(GetPos(), player->GetPos()) < 100)
		{
			switch (type)
			{
			case NPCType::Start:
				if (player->GetIsMove() && !InputMgr::GetKeyDown(Keyboard::Escape))
				{
					isShowMap = true;
					player->SetMove(false);
				}
				else if (isShowMap)
				{
					isShowMap = false;
					player->SetMove(true);
					InputMgr::Clear();
				}
				break;
			case NPCType::Shop:
				if (player->GetIsMove() && !InputMgr::GetKeyDown(Keyboard::Escape))
				{
					isShowShop = true;
					player->SetMove(false);
				}
				else if (isShowShop)
				{
					isShowShop = false;
					player->SetMove(true);
					InputMgr::Clear();
				}
				break;
			case NPCType::Craft:
				if (player->GetIsMove() && !InputMgr::GetKeyDown(Keyboard::Escape))
				{
					isShowCraft = true;
					player->SetMove(false);
				}
				else if (isShowCraft)
				{
					isShowCraft = false;
					player->SetMove(true);
					InputMgr::Clear();
				}
				break;
			case NPCType::Count:
				break;
			default:
				break;
			}
		}
		else
		{
			ment->SetActive(false);
		}
	}
}

void NPC::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
}
