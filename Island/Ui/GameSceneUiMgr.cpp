#include "GameSceneUiMgr.h"
#include "Button.h"
#include "../Framework/ResourceManager.h"
#include "../Framework//info.h"
#include "../Framework/InputMgr.h"
#include "../../Scens/SceneManager.h"
#include "../../Framework/FileManager.h"
#include "../../Scens/GameScene.h"
#include "Inventory.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/Player.h"
#include "../Scens/GameScene.h"
#include "../GameObject/TextObject.h"
#include "../Framework/Framework.h"
#include "../GameObject/NPC.h"
#include "InventoryBox.h"

GameSceneUiMgr::GameSceneUiMgr(Scene* scene)
	:UiMgr(scene), hpBarSize(1.f),staminaBarSize(1.f)
{
}

GameSceneUiMgr::~GameSceneUiMgr()
{
	player->Save();
	Releas();
}

void GameSceneUiMgr::Init()
{
	player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();

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
	hungerTex->SetPos({ 350,40 });
	uiObjList[0].push_back(hungerTex);

	thirstTex = new Button(this);
	thirstTex->SetClkColor(true);
	thirstTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((player->GetThirstGuage() / 255) * 100)), true);
	thirstTex->SetOrigin(Origins::MC);
	thirstTex->SetPos({ 450,40 });
	uiObjList[0].push_back(thirstTex);

	energyTex = new Button(this);
	energyTex->SetClkColor(true);
	energyTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string(((player->GetEnergyGuage() / 255) * 100)), true);
	energyTex->SetOrigin(Origins::MC);
	energyTex->SetPos({ 550,40 });
	uiObjList[0].push_back(energyTex);

	ammoText = new Button(this);
	ammoText->SetClkColor(true);
	ammoText->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		40, Color::White, player->GetAmmos(), true);
	ammoText->SetOrigin(Origins::MC);
	ammoText->SetPos({ 150,200 });
	uiObjList[0].push_back(ammoText);
}

void GameSceneUiMgr::Reset()
{
}

void GameSceneUiMgr::Update(float dt)
{
	//hp bar
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
		thirstTex->GetTextObj()->SetString(to_string((int)result));
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
		energyTex->GetTextObj()->SetString(to_string((result)));
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
	ammoText->GetTextObj()->SetString(player->GetAmmos());
	UiMgr::Update(dt);

}

void GameSceneUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
}

void GameSceneUiMgr::Releas()
{
	UiMgr::Release();
}

