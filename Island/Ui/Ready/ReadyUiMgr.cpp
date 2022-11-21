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

ReadyUiMgr::ReadyUiMgr(Scene* scene)
	:UiMgr(scene)
{

}

void ReadyUiMgr::Init()
{
	player = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetPlayer();
	npc = ((Ready*)(SCENE_MGR->GetCurrScene()))->GetNPC();

	mapsBK = new Button(this);
	mapsBK->SetTexture(*RESOURCES_MGR->GetTexture("graphics/mapsbk.png"), false);
	mapsBK->SetPos({
		(float)FRAMEWORK->GetWindowSize().x / 2,(float)FRAMEWORK->GetWindowSize().y / 2 });
	mapsBK->SetOrigin(Origins::MC);
	mapsBK->SetActive(false);
	uiObjList[0].push_back(mapsBK);

	auto map = new Button(this);
	map->SetClkColor(true);
	map->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 200, Color::White
		, "map1", true);
	map->SetPos({ 400,300 });
	maps.push_back(map);

	auto map1 = new Button(this);
	map1->SetClkColor(true);
	map1->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 200, Color::White
		, "map2", true);
	map1->SetPos({ 1000,300 });
	maps.push_back(map1);

	for (auto& map : maps)
	{
		uiObjList[0].push_back(map);
	}
}

void ReadyUiMgr::Update(float dt)
{
	UiMgr::Update(dt);
	if (npc->GetShowMap())
	{
		mapsBK->SetActive(true);
		for (auto& map : maps)
		{
			map->SetActive(true);
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
}

void ReadyUiMgr::Draw(RenderWindow& window)
{
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