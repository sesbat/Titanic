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

GameSceneUiMgr::GameSceneUiMgr(Scene* scene)
	:UiMgr(scene), hpBarSize(1.f), staminaBarSize(1.f), time(1.0f),
	hungerGuage(255), thirstGuage(255), energyGuage(255), dash(0.01f),
	prevHungerGuage(255)
{
}

GameSceneUiMgr::~GameSceneUiMgr()
{
}

void GameSceneUiMgr::Init()
{
	//hp
	hpBar = new Button(this);
	hpBar->SetClkColor(false);
	hpBar->SetTexture(*RESOURCES_MGR->GetTexture("graphics/hpbar.png"),false);
	hpBar->SetOrigin(Origins::ML);
	hpBar->SetPos({300,100});
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
	hunger->GetSpriteObj()->SetColor(Color(169, 57, 53, (int)hungerGuage));
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
	thirst->GetSpriteObj()->SetColor(Color(0, 145, 255, (int)thirstGuage));
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
	energy->GetSpriteObj()->SetColor(Color(235, 255, 0, (int)energyGuage));
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

	//text
	exitBtn = new Button(this);
	exitBtn->SetClkColor(true);
	exitBtn->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		75, Color::White, "EXIT", true);
	exitBtn->SetOrigin(Origins::TL);
	exitBtn->SetPos({ 50,50 });
	uiObjList[0].push_back(exitBtn);
	inven = new Inventory(this);
	inven->Init();
	uiObjList[1].push_back(inven);
	inven->SetActive(false);
	

	hungerTex = new Button(this);
	hungerTex->SetClkColor(true);
	hungerTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string((int)((hungerGuage / 255) * 100)), true);
	hungerTex->SetOrigin(Origins::MC);
	hungerTex->SetPos({ 500,50 });
	uiObjList[0].push_back(hungerTex);

	thirstTex = new Button(this);
	thirstTex->SetClkColor(true);
	thirstTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string((int)((thirstGuage / 255) * 100)), true);
	thirstTex->SetOrigin(Origins::MC);
	thirstTex->SetPos({ 600,50 });
	uiObjList[0].push_back(thirstTex);

	energyTex = new Button(this);
	energyTex->SetClkColor(true);
	energyTex->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
		20, Color::White, to_string((int)((energyGuage / 255) * 100)), true);
	energyTex->SetOrigin(Origins::MC);
	energyTex->SetPos({ 700,50 });
	uiObjList[0].push_back(energyTex);

	player = ((GameScene*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
}

void GameSceneUiMgr::Reset()
{
}

void GameSceneUiMgr::Update(float dt)
{
	//hp bar
	hpBarSize = (float)player->GetHp() * 0.1f;
	hpBar->GetSpriteObj()->SetScale({ hpBarSize,1.f });

	//dev input
	if (InputMgr::GetKeyDown(Keyboard::Num0))
	{
		hungerGuage = 255;
	}
	if (InputMgr::GetKeyDown(Keyboard::Num9))
	{
		hpBarSize -= 0.1f;
	}

	//stamina
	staminaBarSize = player->GetStamina() * 0.1f;
	staminaBar->GetSpriteObj()->SetScale({ staminaBarSize,1.f });

	//hunger guage
	hungerGuage -= 0.1f;
	if (hungerGuage <= 0.f)
	{
		hungerGuage = 0.f;
	}
	if ((int)hungerGuage != prevHungerGuage)
	{
		prevHungerGuage = (int)hungerGuage;
		int result = (int)((hungerGuage / 255) * 100);
		hungerTex->GetTextObj()->SetString(to_string(result));
		hunger->GetSpriteObj()->SetColor(Color(169, 57, 53, hungerGuage));
		if (result < 30.f)
		{
			hungerTex->GetTextObj()->SetColor(Color::Red);
		}
		else
		{
			hungerTex->GetTextObj()->SetColor(Color::White);
		}
	}

	UiMgr::Update(dt);

	if (InputMgr::GetKeyDown(Keyboard::Tab))
	{
		inven->SetActive(!(inven->GetActive()));
	}
}

void GameSceneUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
}


bool GameSceneUiMgr::IsExit()
{
	return exitBtn->IsDown() || exitBtn->IsClick();
}
