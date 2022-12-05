#include "ReadyUiMgr.h"
#include "../../Scens/Scene.h"
#include "ReadyButton.h"
#include "../../GameObject/SpriteObject.h"
#include "../../Framework/ResourceManager.h"
#include "../../Framework/info.h"
#include "../../Scens/SceneManager.h"
#include "../../GameObject/NPC.h"
#include "../../Scens/Ready.h"
#include "../../Framework/Framework.h"
#include "../UiMgr.h"
#include "../../Framework/FileManager.h"
#include "../../GameObject/Player.h"
#include "../../Ui/Inventory.h"
#include "../Craft.h"
#include "../Button.h"
#include "../../GameObject/TextObject.h"
#include "../Shop/Shop.h"

ReadyUiMgr::ReadyUiMgr(Scene* scene)
	:UiMgr(scene)
{

}

ReadyUiMgr::~ReadyUiMgr()
{
	Release();
}

void ReadyUiMgr::Init()
{
	player = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
	startNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetStartNPC();
	craftNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetCraftNPC();
	shopNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetShopNPC();

	//hp
	hpBar = new Button(this);
	hpBar->SetClkColor(false);
	hpBar->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpbar.png"), false);
	hpBar->SetOrigin(Origins::ML);
	hpBar->SetPos({ 300,100 });
	uiObjList[0].push_back(hpBar);

	hpBarBK = new Button(this);
	hpBarBK->SetClkColor(false);
	hpBarBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpbarbk.png"), false);
	hpBarBK->SetOrigin(Origins::ML);
	hpBarBK->SetPos({ 300,100 });
	uiObjList[0].push_back(hpBarBK);

	hpSprite = new Button(this);
	hpSprite->SetClkColor(false);
	hpSprite->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpsprite.png"), false);
	hpSprite->SetOrigin(Origins::MC);
	hpSprite->GetSpriteObj()->SetScale({ 0.8f,0.8f });
	hpSprite->SetPos({ 250,100 });
	uiObjList[0].push_back(hpSprite);

	//stamina
	staminaBar = new Button(this);
	staminaBar->SetClkColor(false);
	staminaBar->SetTexture(*RESOURCES_MGR->GetTexture("graphics/stamina.png"), false);
	staminaBar->SetOrigin(Origins::ML);
	staminaBar->SetPos({ 300,150 });
	uiObjList[0].push_back(staminaBar);

	staminaBK = new Button(this);
	staminaBK->SetClkColor(false);
	staminaBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/staminabk.png"), false);
	staminaBK->SetOrigin(Origins::ML);
	staminaBK->SetPos({ 300,150 });
	uiObjList[0].push_back(staminaBK);

	staminaSprite = new Button(this);
	staminaSprite->SetClkColor(false);
	staminaSprite->SetTexture(*RESOURCES_MGR->GetTexture("graphics/staminasprite.png"), false);
	staminaSprite->SetOrigin(Origins::MC);
	staminaSprite->GetSpriteObj()->SetScale({ 0.5f,0.5f });
	staminaSprite->SetPos({ 250,150 });
	uiObjList[0].push_back(staminaSprite);

	//hunger
	hunger = new Button(this);
	hunger->SetClkColor(false);
	hunger->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hunger.png"), false);
	hunger->GetSpriteObj()->SetColor(Color(169, 57, 53, (int)player->GetHungerGuage()));
	hunger->SetOrigin(Origins::MC);
	//hunger->GetSpriteObj()->SetScale({ 0.5f,0.5f });
	hunger->SetPos({ 500,100 });
	uiObjList[0].push_back(hunger);

	hungerBK = new Button(this);
	hungerBK->SetClkColor(false);
	hungerBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hungerbk.png"), false);
	hungerBK->SetOrigin(Origins::MC);
	hungerBK->SetPos({ 500,100 });
	uiObjList[0].push_back(hungerBK);

	//thirst
	thirst = new Button(this);
	thirst->SetClkColor(false);
	thirst->SetTexture(*RESOURCES_MGR->GetTexture("graphics/thirst.png"), false);
	thirst->GetSpriteObj()->SetColor(Color(0, 145, 255, (int)player->GetThirstGuage()));
	thirst->SetOrigin(Origins::MC);
	thirst->SetPos({ 600,100 });
	uiObjList[0].push_back(thirst);

	thirstBK = new Button(this);
	thirstBK->SetClkColor(false);
	thirstBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/thirstbk.png"), false);
	thirstBK->SetOrigin(Origins::MC);
	thirstBK->SetPos({ 600,100 });
	uiObjList[0].push_back(thirstBK);

	//energy
	energy = new Button(this);
	energy->SetClkColor(false);
	energy->SetTexture(*RESOURCES_MGR->GetTexture("graphics/energy.png"), false);
	energy->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)player->GetEnergyGuage()));
	energy->SetOrigin(Origins::MC);
	energy->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	energy->SetPos({ 700,100 });
	uiObjList[0].push_back(energy);

	energyBK = new Button(this);
	energyBK->SetClkColor(false);
	energyBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/energybk.png"), false);
	energyBK->SetOrigin(Origins::MC);
	energyBK->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	energyBK->SetPos({ 700,100 });
	uiObjList[0].push_back(energyBK);

	hungerTex = new Button(this);
	hungerTex->SetClkColor(true);
	hungerTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((int)(player->GetHungerGuage() / 255) * 100)), true);
	hungerTex->SetOrigin(Origins::MC);
	hungerTex->SetPos({ 490,50 });
	uiObjList[0].push_back(hungerTex);

	thirstTex = new Button(this);
	thirstTex->SetClkColor(true);
	thirstTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((int)(player->GetThirstGuage() / 255) * 100)), true);
	thirstTex->SetOrigin(Origins::MC);
	thirstTex->SetPos({ 590,50 });
	uiObjList[0].push_back(thirstTex);

	energyTex = new Button(this);
	energyTex->SetClkColor(true);
	energyTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((int)(player->GetEnergyGuage() / 255) * 100)), true);
	energyTex->SetOrigin(Origins::MC);
	energyTex->SetPos({ 690,50 });
	uiObjList[0].push_back(energyTex);

	mapsBK = new Button(this);
	mapsBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/mapsbk.png"), false);
	mapsBK->SetPos({
		(float)FRAMEWORK->GetWindowSize().x / 2,(float)FRAMEWORK->GetWindowSize().y / 2 });
	mapsBK->SetOrigin(Origins::MC);
	mapsBK->SetActive(false);
	uiObjList[0].push_back(mapsBK);

	craft = new Craft(this);
	craft->Init();
	uiObjList[0].push_back(craft);

	shop = new Shop(this);
	shop->Init();
	uiObjList[0].push_back(shop);

	categoryBK = new Button(this);
	categoryBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/craftbk.png"),false);
	categoryBK->SetPos({
		(float)FRAMEWORK->GetWindowSize().x / 2,(float)FRAMEWORK->GetWindowSize().y / 2 });
	categoryBK->SetOrigin(Origins::MC);
	categoryBK->SetActive(false);
	uiObjList[0].push_back(categoryBK);


	auto& mapData = FILE_MGR->GetAllMap();
	float tmpY = 300.f;
	for (auto& data : mapData)
	{
		if (data.first == "READYSCENE")
			continue;
		auto map = new Button(this);
		map->SetClkColor(true);
		map->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 50, Color::White
			,data.first, true);
		map->SetPos({ 400,tmpY });
		tmpY += 50.f;
		map->SetName(data.first);
		maps.push_back(map);
	}

	for (auto& map : maps)
	{
		uiObjList[0].push_back(map);
	}

	uiObjList[0].push_back(player->GetInventory());
}

void ReadyUiMgr::Update(float dt)
{
	UiMgr::Update(dt);

	hpBarSize = (float)player->GetHp() / player->GetMaxHp();// *0.002f;
	hpBar->GetSpriteObj()->SetScale({ hpBarSize,1.f });

	//stamina
	staminaBarSize = player->GetStamina() * 0.1f;
	staminaBar->GetSpriteObj()->SetScale({ staminaBarSize,1.f });

	if (!player->Hunger())
	{
		player->SetPrevHungerGuage(player->GetHungerGuage());
		int result = ((player->GetHungerGuage() / 255) * 100);
		hungerTex->GetTextObj()->SetString(to_string(result));
		hunger->GetSpriteObj()->SetColor(Color(169, 57, 53, (int)player->GetHungerGuage()));
		if (result < 30.f)
		{
			hungerTex->GetTextObj()->SetColor(Color::Red);
		}
		else
		{
			hungerTex->GetTextObj()->SetColor(Color::White);
		}
	}
	if (!player->Thirst())
	{
		player->SetPrevThirstGuage(player->GetThirstGuage());
		int result = ((player->GetThirstGuage() / 255) * 100);
		thirstTex->GetTextObj()->SetString(to_string(result));
		thirst->GetSpriteObj()->SetColor(Color(0, 145, 255, (int)player->GetThirstGuage()));
		if (result < 30.f)
		{
			thirstTex->GetTextObj()->SetColor(Color::Red);
		}
		else
		{
			thirstTex->GetTextObj()->SetColor(Color::White);
		}
	}
	if (!player->Energy())
	{
		player->SetPrevEnergyGuage(player->GetEnergyGuage());
		int result = ((player->GetEnergyGuage() / 255) * 100);
		energyTex->GetTextObj()->SetString(to_string(result));
		energy->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)player->GetEnergyGuage()));
		if (result < 30.f)
		{
			energyTex->GetTextObj()->SetColor(Color::Red);
		}
		else
		{
			energyTex->GetTextObj()->SetColor(Color::White);
		}
	}

	if (startNpc->GetShowMap())
	{
		mapsBK->SetActive(true);
		for (auto& map : maps)
		{
			map->SetActive(true);

			if (map->IsUp())
			{
				SCENE_MGR->ChangeScene(Scenes::GameScene, map->GetName());
				return;
			}
		}
	}
	else
	{
		mapsBK->SetActive(false);
		for (auto& map : maps)
		{
			map->SetActive(false);
		}
	}
	if (craftNpc->GetShowCraft())
	{
		craft->SetActive(true);
	}
	else
	{
		craft->SetActive(false);
	}

	if (shopNpc->GetShowShop())
	{
		shop->SetActive(true);
	}
	else
	{
		shop->SetActive(false);
	}
}

void ReadyUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
}

void ReadyUiMgr::Release()
{
	UiMgr::Release();
}
