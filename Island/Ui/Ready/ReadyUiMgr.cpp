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
#include "../BoolWindowBox.h"
#include "../../GameObject/ToolTip.h"

ReadyUiMgr::ReadyUiMgr(Scene* scene)
	:UiMgr(scene), messageTime(3.f)
{

}

ReadyUiMgr::~ReadyUiMgr()
{
	Release();
}

void ReadyUiMgr::Init()
{
	tip = new ToolTip(this);
	uiObjList[1].push_back(tip);

	player = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
	startNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetStartNPC();
	craftNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetCraftNPC();
	shopNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetShopNPC();
	healNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetHealNPC();

	yesORno = new BoolWindowBox(this);
	yesORno->SetPos({(float) FRAMEWORK->GetWindowSize().x/2,(float)FRAMEWORK->GetWindowSize().y/2});
	yesORno->SetOrigin(Origins::MC);
	yesORno->Init();
	yesORno->SetActive(false);
	uiObjList[1].push_back(yesORno);

	

	//hp
	hpBar = new Button(this);
	hpBar->SetClkColor(false);
	hpBar->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpbar.png"), false);
	hpBar->SetOrigin(Origins::ML);
	hpBar->SetPos({ 100,100 });
	uiObjList[0].push_back(hpBar);

	hpBarBK = new Button(this);
	hpBarBK->SetClkColor(false);
	hpBarBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpbarbk.png"), false);
	hpBarBK->SetOrigin(Origins::ML);
	hpBarBK->SetPos({ 100,100 });
	uiObjList[0].push_back(hpBarBK);

	hpSprite = new Button(this);
	hpSprite->SetClkColor(false);
	hpSprite->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpsprite.png"), false);
	hpSprite->SetOrigin(Origins::MC);
	hpSprite->GetSpriteObj()->SetScale({ 0.8f,0.8f });
	hpSprite->SetPos({ 50,100 });
	uiObjList[0].push_back(hpSprite);

	//stamina
	staminaBar = new Button(this);
	staminaBar->SetClkColor(false);
	staminaBar->SetTexture(*RESOURCES_MGR->GetTexture("graphics/stamina.png"), false);
	staminaBar->SetOrigin(Origins::ML);
	staminaBar->SetPos({ 100,150 });
	uiObjList[0].push_back(staminaBar);

	staminaBK = new Button(this);
	staminaBK->SetClkColor(false);
	staminaBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/staminabk.png"), false);
	staminaBK->SetOrigin(Origins::ML);
	staminaBK->SetPos({ 100,150 });
	uiObjList[0].push_back(staminaBK);

	staminaSprite = new Button(this);
	staminaSprite->SetClkColor(false);
	staminaSprite->SetTexture(*RESOURCES_MGR->GetTexture("graphics/staminasprite.png"), false);
	staminaSprite->SetOrigin(Origins::MC);
	staminaSprite->GetSpriteObj()->SetScale({ 0.5f,0.5f });
	staminaSprite->SetPos({ 50,150 });
	uiObjList[0].push_back(staminaSprite);

	//hunger
	hunger = new Button(this);
	hunger->SetClkColor(false);
	hunger->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hunger.png"), false);
	hunger->GetSpriteObj()->SetColor(Color(169, 57, 53, (int)player->GetHungerGuage()));
	hunger->SetOrigin(Origins::MC);
	//hunger->GetSpriteObj()->SetScale({ 0.5f,0.5f });
	hunger->SetPos({ 350,100 });
	uiObjList[0].push_back(hunger);

	hungerBK = new Button(this);
	hungerBK->SetClkColor(false);
	hungerBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hungerbk.png"), false);
	hungerBK->SetOrigin(Origins::MC);
	hungerBK->SetPos({ 350,100 });
	uiObjList[0].push_back(hungerBK);

	//thirst
	thirst = new Button(this);
	thirst->SetClkColor(false);
	thirst->SetTexture(*RESOURCES_MGR->GetTexture("graphics/thirst.png"), false);
	thirst->GetSpriteObj()->SetColor(Color(0, 145, 255, (int)player->GetThirstGuage()));
	thirst->SetOrigin(Origins::MC);
	thirst->SetPos({ 450,100 });
	uiObjList[0].push_back(thirst);

	thirstBK = new Button(this);
	thirstBK->SetClkColor(false);
	thirstBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/thirstbk.png"), false);
	thirstBK->SetOrigin(Origins::MC);
	thirstBK->SetPos({ 450,100 });
	uiObjList[0].push_back(thirstBK);

	//energy
	energy = new Button(this);
	energy->SetClkColor(false);
	energy->SetTexture(*RESOURCES_MGR->GetTexture("graphics/energy.png"), false);
	energy->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)player->GetEnergyGuage()));
	energy->SetOrigin(Origins::MC);
	energy->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	energy->SetPos({ 550,100 });
	uiObjList[0].push_back(energy);

	energyBK = new Button(this);
	energyBK->SetClkColor(false);
	energyBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/energybk.png"), false);
	energyBK->SetOrigin(Origins::MC);
	energyBK->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	energyBK->SetPos({ 550,100 });
	uiObjList[0].push_back(energyBK);

	hungerTex = new Button(this);
	hungerTex->SetClkColor(true);
	hungerTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((player->GetHungerGuage() / 255) * 100)), true);
	hungerTex->SetOrigin(Origins::MC);
	hungerTex->SetPos({ 380,40 });
	uiObjList[0].push_back(hungerTex);

	thirstTex = new Button(this);
	thirstTex->SetClkColor(true);
	thirstTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((player->GetThirstGuage() / 255) * 100)), true);
	thirstTex->SetOrigin(Origins::MC);
	thirstTex->SetPos({ 480,40 });
	uiObjList[0].push_back(thirstTex);

	energyTex = new Button(this);
	energyTex->SetClkColor(true);
	energyTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((player->GetEnergyGuage() / 255) * 100)), true);
	energyTex->SetOrigin(Origins::MC);
	energyTex->SetPos({ 580,40 });
	uiObjList[0].push_back(energyTex);

	radiation = new Button(this);
	radiation->SetClkColor(false);
	radiation->SetTexture(*RESOURCES_MGR->GetTexture("graphics/rad.png"), false);
	radiation->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)player->GetRadiGuage()));
	radiation->SetOrigin(Origins::MC);
	radiation->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	radiation->SetPos({ 650,100 });
	uiObjList[0].push_back(radiation);


	radiationBK = new Button(this);
	radiationBK->SetClkColor(false);
	radiationBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/radbk.png"), false);
	radiationBK->SetOrigin(Origins::MC);
	radiationBK->GetSpriteObj()->SetScale({ 1.5f,1.5f });
	radiationBK->SetPos({ 650,100 });
	uiObjList[0].push_back(radiationBK);

	radiationTex = new Button(this);
	radiationTex->SetClkColor(false);
	radiationTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string((int)((player->GetRadiGuage() / 255) * 100)), true);
	radiationTex->SetOrigin(Origins::MC);
	radiationTex->SetPos({ 680,40 });
	uiObjList[0].push_back(radiationTex);

	healed = new Button(this);
	healed->SetPos({ (float)FRAMEWORK->GetWindowSize().x / 2,(float)FRAMEWORK->GetWindowSize().y / 2 -200 });
	healed->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		60, Color::White, "You have been healed",false);
		healed->SetOrigin(Origins::MC);
	healed->Init();
	healed->SetActive(false);
	uiObjList[1].push_back(healed);

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

	auto playerClearMaps = player->GetClearMpas();
	auto connecntMaps = FILE_MGR->GetConnecntInfoAll();

	for (auto& data : mapData)
	{
		if (data.first == "READYSCENE")
			continue;
		auto map = new Button(this);
		map->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 50, Color::White
			,data.first, true);
		map->SetPos({ 400,tmpY });
		tmpY += 50.f;
		map->SetName(data.first);
		maps.push_back(map);

		auto& needMaps = connecntMaps[data.first];
		bool isLock = false;

		for (auto& find_map : needMaps)
		{
			if (find(playerClearMaps.begin(), playerClearMaps.end(), find_map) == playerClearMaps.end())
			{
				isLock = true;
				break;
			}
		}

		map->SetClkColor(!isLock);
		map->GetTextObj()->SetColor(isLock ? Color(180,180,180,255) : Color::White);
		
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
	player->SetStamina(10.f);
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
	if (!player->Radiation())
	{
		player->SetPrevRadGuage(player->GetRadiGuage());
		int result = ((player->GetRadiGuage() / 255) * 100);
		radiationTex->GetTextObj()->SetString(to_string((int)(result)));
		radiation->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)player->GetRadiGuage()));
		if (result > 125.f)
		{
			radiationTex->GetTextObj()->SetColor(Color::Red);
		}
		else
		{
			radiationTex->GetTextObj()->SetColor(Color::White);
		}
	}

	if (startNpc->GetShowMap())
	{
		mapsBK->SetActive(true);
		for (auto& map : maps)
		{
			map->SetActive(true);

			if (map->GetIsClickColor() && map->IsUp())
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
	if (healNpc->GetIsHeal())
	{
		yesORno->SetActive(true);
	}
	else
	{
		yesORno->SetActive(false);
	}
	if (yesORno->GetActive())
	{
		if (yesORno->GetYes() &&
			(player->GetHp() < player->GetMaxHp() ||
				player->GetRadiGuage() > 0.f ||
				player->GetThirstGuage() < 255.f ||
				player->GetHungerGuage() < 255.f ||
				player->GetEnergyGuage() < 255.f) &&
			player->GetMoney() >= 500)
		{
			player->AddMoney(-500 /*+ (500 * (player->GetHp() / player->GetMaxHp()))*/);
			player->HealHunger(255.f);
			player->HealEnergy(255.f);
			player->HealThirst(255.f);
			player->HealRad(-255.f);
			player->HealHp(player->GetMaxHp());
			yesORno->SetYes(false);
			healNpc->SetIsHeal(false);
			player->SetMove(true);
		}
		if (yesORno->GetYes()&&player->GetHp()>=player->GetMaxHp())
		{
			//you have been healed;
			healed->SetActive(true);
		}
		if (yesORno->GetNo())
		{
			yesORno->SetNo(false);
			healNpc->SetIsHeal(false);
			player->SetMove(true);
		}
	}
	if (healed->GetActive())
	{
		messageTime -= dt;
		if (messageTime <= 0.f)
		{
			healed->SetActive(false);
			messageTime = 3.f;
		}
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
