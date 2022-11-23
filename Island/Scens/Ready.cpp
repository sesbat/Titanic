#include "Ready.h"
#include "SceneManager.h"
#include "../../Framework/InputMgr.h"
#include "../../Framework/ResourceManager.h"
#include "../../GameObject/SpriteObject.h"
#include "../../GameObject/TextObject.h"
#include "../../Framework/Utils.h"
#include "../../Framework/info.h"
#include "../../Ui/Ready/ReadyUiMgr.h"
#include "../../Framework/SoundManager.h"
#include "../GameObject/NPC.h"
#include "../GameObject/HitBoxObject.h"
#include "../GameObject/Player.h"
#include "../Framework/Framework.h"

Ready::Ready()
	:Scene(Scenes::Ready)
{

}

Ready::~Ready()
{
	Release();
}

void Ready::Init()
{
	int id = 0;
	isMap = true;
	auto& data = FILE_MGR->GetMap("READYSCENE");

	for (auto& obj : data)
	{
		if (obj.type == "TREE" || obj.type == "BUSH" || obj.type == "STONE" || obj.type == "BLOCK")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);

			objList[LayerType::Object][0].push_back(draw);
		}
		else if (obj.type == "PLAYER")
		{
			player = new Player();
			player->SetName(obj.type);
			player->Init();
			player->SetPos(obj.position);
			player->SetHitBox(obj.path);

			objList[LayerType::Object][0].push_back(player);
		}
		else if (obj.type == "TILE")
		{
			HitBoxObject* draw = new HitBoxObject();
			draw->SetName(obj.type);
			draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
			draw->SetOrigin(Origins::BC);
			draw->SetPos(obj.position);
			draw->SetHitBox(obj.path);

			objList[LayerType::Tile][0].push_back(draw);
		}
	}

	auto& tiles = objList[LayerType::Tile][0];
	mapSize.left = 0;
	mapSize.top = 0;
	mapSize.width = (tiles.back())->GetPos().x + 30;
	mapSize.height = (tiles.back())->GetPos().y;

	npc = new NPC();
	npc->SetTexture(*RESOURCES_MGR->GetTexture("graphics/npc.png"));
	npc->SetOrigin(Origins::BC);
	npc->SetPlayer(player);
	npc->SetPos({ 990.f,1740.f });
	npc->SetName("NPC");
	npc->Init();
	npc->SetHitBox("graphics/player.png");
	objList[LayerType::Object][0].push_back(npc);

	//npc = new NPC();
	//npc->SetTexture(*RESOURCES_MGR->GetTexture("graphics/npc.png"));
	//npc->SetOrigin(Origins::MC);
	//npc->SetPlayer(player);
	//npc->SetScale({ 3.f,3.f });
	//npc->SetPos({ 100.f,100.f });
	//npc->SetName("NPC");
	//npc->Init();
	//objList[LayerType::Object][0].push_back(npc);

	uiMgr = new ReadyUiMgr(this);
	uiMgr->Init();
}

void Ready::Release()
{
	Scene::Release();
}

void Ready::Enter()
{
	Init();

	SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
	SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
	SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });

	SOUND_MGR->StopAll();
}

void Ready::Exit()
{
	Release();
}

void Ready::Update(float dt)
{
	LayerSort();


	Vector2f mouseworldPos = FRAMEWORK->GetWindow().mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), worldView);

	Vector2f dir;
	dir.x = mouseworldPos.x - player->GetPos().x;
	dir.y = mouseworldPos.y - player->GetPos().y;

	//camera
	float r = 0.1;
	Vector2f camPoslen;
	camPoslen.x = dir.x * r;
	camPoslen.y = dir.y * r;
	Vector2f realcam;
	realcam.x = camPoslen.x + player->GetPos().x;
	realcam.y = camPoslen.y + player->GetPos().y;

	realcam.x = max((int)realcam.x, WINDOW_WIDTH / 2);
	realcam.x = min((int)realcam.x, mapSize.width - WINDOW_WIDTH / 2);
	realcam.y = max((int)realcam.y, WINDOW_HEIGHT / 2);
	realcam.y = min((int)realcam.y, mapSize.height - WINDOW_HEIGHT / 2);

	worldView.setCenter(realcam);

	Scene::Update(dt);
}

void Ready::Draw(RenderWindow& window)
{
	Scene::Draw(window);
}

void Ready::Reset()
{
	uiMgr->Init();
}
