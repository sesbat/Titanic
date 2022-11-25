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

ReadyUiMgr::ReadyUiMgr(Scene* scene)
	:UiMgr(scene)
{

}

void ReadyUiMgr::Init()
{
	player = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
	startNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetStartNPC();
	craftNpc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetCraftNPC();

	inVen = player->GetInventory();
	uiObjList[1].push_back(inVen);

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
		tmpY -= 50.f;
		map->SetName(data.first);
		maps.push_back(map);
	}
	//auto map = new Button(this);
	//map->SetClkColor(true);
	//map->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 200, Color::White
	//	, "map1", true);
	//map->SetPos({ 400,300 });
	//maps.push_back(map);

	//auto map1 = new Button(this);
	//map1->SetClkColor(true);
	//map1->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 200, Color::White
	//	, "map2", true);
	//map1->SetPos({ 1000,300 });
	//maps.push_back(map1);

	for (auto& map : maps)
	{
		uiObjList[0].push_back(map);
	}
}

void ReadyUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	if (startNpc->GetShowMap())
	{
		mapsBK->SetActive(true);
		for (auto& map : maps)
		{
			map->SetActive(true);

			if (map->IsUp())
			{
				SCENE_MGR->ChangeScene(Scenes::GameScene, map->GetName());
				break;
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
}

void ReadyUiMgr::Draw(RenderWindow& window)
{
	window.setView(parentScene->GetUiView());
	UiMgr::Draw(window);
}

void ReadyUiMgr::Release()
{
}

void ReadyUiMgr::ClickEpisodeBtn()
{
}

void ReadyUiMgr::ClickEditortBtn()
{
}