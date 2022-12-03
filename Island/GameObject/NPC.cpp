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
	SpriteObject::Release();
}

void NPC::Update(float dt)
{
	SpriteObject::Update(dt);
	//cout << GetPos().x<<" "<< GetPos().y << endl;


	if (!ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) < 100)
	{
		ment->SetActive(true);
	}
	else if(ment->GetActive() && Utils::Distance(GetPos(), player->GetPos()) >= 100)
	{
		ment->SetActive(false);
	}

	if (InputMgr::GetKeyDown(Keyboard::F))
	{
		if (Utils::Distance(GetPos(), player->GetPos()) < 100)
		{
			switch (type)
			{
			case NPCType::Start:
				if (player->GetIsMove())
				{
					isShowMap = true;
					player->SetMove(false);
				}
				else if (isShowMap)
				{
					isShowMap = false;
					player->SetMove(true);
				}
				break;
			case NPCType::Shop:
				if (player->GetIsMove())
				{
					isShowShop = true;
					player->SetMove(false);
				}
				else if (isShowShop)
				{
					isShowShop = false;
					player->SetMove(true);
				}
				break;
			case NPCType::Craft:
				if (player->GetIsMove())
				{
					isShowCraft = true;
					player->SetMove(false);
				}
				else if (isShowCraft)
				{
					isShowCraft = false;
					player->SetMove(true);
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

	//if (InputMgr::GetKeyDown(Keyboard::F))
	//{
	//	if (isShowMap && !player->GetIsMove())
	//	{
	//		switch (type)
	//		{
	//		case NPCType::Start:
	//			isShowMap = false;
	//			break;
	//		case NPCType::Shop:
	//			break;
	//		case NPCType::Craft:
	//			isShowCraft = false;
	//			break;
	//		case NPCType::Count:
	//			break;
	//		default:
	//			break;
	//		}

	//		player->SetMove(true);
	//	}
	//	else if (!isShowMap && player->GetIsMove() && Utils::Distance(GetPos(), player->GetPos()) < 100)
	//	{
	//		switch (type)
	//		{
	//		case NPCType::Start:
	//			isShowMap = true;
	//			break;
	//		case NPCType::Shop:
	//			break;
	//		case NPCType::Craft:
	//			isShowCraft = true;
	//			break;
	//		case NPCType::Count:
	//			break;
	//		default:
	//			break;
	//		}
	//		player->SetMove(false);
	//	}
	//}
}

void NPC::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
}
