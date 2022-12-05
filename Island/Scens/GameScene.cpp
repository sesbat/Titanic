#include "GameScene.h"
#include "SceneManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/SoundManager.h"
#include "../Framework/Framework.h"
#include "../GameObject/Player.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/VertexArrayObj.h"
#include "../../Framework/info.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../GameObject/HitBoxObject.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include "../Ui/GameSceneUiMgr.h"
#include "../GameObject/NPC.h"
#include "../GameObject/ItemBoxObject.h"
#include "../Ui/Inventory.h"
#include "../Ui/InventoryBox.h"
#include "Candle/geometry/Polygon.hpp"

#include "../GameObject/HitBox.h"

using namespace std;
using namespace sf;

//explicit QuadTree(sf::FloatRect bounds,   size_t maxLevel, size_t maxObjects);
GameScene::GameScene()
    :Scene(Scenes::GameScene), timer(0.f), escapeTimer(3.f),
    /*   fog(candle::LightingArea::FOG,
          sf::Vector2f(0.f, 0.f),
          sf::Vector2f(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2)),*/ blockCount(0), treeMap(treeRect, 16, 4)
{

}

GameScene::~GameScene()
{
    treeMap.clear();
}

void GameScene::Init()
{
    int id = 0;
    isGameScene = true;
    auto& data = FILE_MGR->GetMap(sceneName);
    isGreedObject.clear();

    for (int i = 0; i < 72; i++)
    {
        isGreedObject.push_back(vector<bool>(128, false));
        objList[LayerType::Tile][i] = vector<Object*>(128, nullptr);
    }

    player = new Player();
    player->SetName("PLAYER");
    player->Init();

    if (InputMgr::GetKeyDown(Keyboard::Escape))
    {
        SCENE_MGR->ChangeScene(Scenes::Menu);
    }

    for (auto& obj : data)
    {
        if (obj.type == "STONE" || obj.type == "BLOCK")
        {
            isGreedObject[obj.greedIdx.x][obj.greedIdx.y] = true;
            Blocks block = { obj.position, pushBlock(obj.position) };
            blockPool.push_back(block);
            //blockCount++;
        }
        if (obj.type == "BOX" || obj.type == "BOX-ENEMY")
        {
            ItemBoxObject* box = new ItemBoxObject();
            box->SetPlayerPos(player->GetPosPtr());
            box->SetItems(obj.item);
            box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/box.png"));
            box->SetName(obj.type);
            box->SetOrigin(Origins::BC);
            box->SetPos(obj.position);
            box->SetHitBox(obj.path);
            objList[LayerType::Object][0].push_back(box);
        }
        else if (obj.type == "TREE" || obj.type == "BUSH" ||
            obj.type == "STONE" || obj.type == "BLOCK")
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
            player->SetPos(obj.position);
            player->SetHitBox(obj.path);

            objList[LayerType::Object][0].push_back(player);
        }
        else if (obj.type == "ENEMY")
        {
            Enemy* enemy = new Enemy();
            enemy->SetName(obj.type);
            enemy->SetId(id++);
            enemy->SetPos(obj.position);
            enemy->SetHitBox(obj.path);
            if (obj.path == "graphics/enemy1.png")
            {
                enemy->SetType(1);
            }
            else if (obj.path == "graphics/enemy2.png")
            {
                enemy->SetType(2);
            }
            else if (obj.path == "graphics/enemy3.png")
            {
                enemy->SetType(3);
            }
            enemy->SetItem(obj.item);
            enemy->SetGreedObject(&isGreedObject);
            enemies.push_back(enemy);

            objList[LayerType::Object][0].push_back(enemy);
        }
        else if (obj.type == "TILE")
        {
            HitBoxObject* draw = new HitBoxObject();
            draw->SetName(obj.type);
            draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
            draw->SetOrigin(Origins::BC);
            draw->SetPos(obj.position);
            draw->SetHitBox(obj.path);

            objList[LayerType::Tile][obj.greedIdx.x][obj.greedIdx.y] = draw;
        }
        else if (obj.type == "ANOTHER")
        {
            HitBoxObject* draw = new HitBoxObject();
            draw->SetName(obj.type);
            draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
            draw->SetOrigin(Origins::BC);
            draw->SetPos(obj.position);
            draw->SetHitBox(obj.path);
            escapePoint = obj.position;
            escapePoint.y -= 100.f;

            objList[LayerType::Object][0].push_back(draw);

            missionText = new TextObject();
            missionText->SetActive(false);
            missionText->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 80, Color::Green, to_string(escapeTimer));
            missionText->SetTextLine(Color::Black, 1.f);
            missionText->SetOrigin(Origins::BC);
            missionText->SetPos({ escapePoint.x - 150.f,escapePoint.y });
            objList[LayerType::Object][1].push_back(missionText);
            //HitBoxObject* exit = new HitBoxObject();
            //exit->SetTexture(*RESOURCES_MGR->GetTexture("graphics/exit.png"));
            //exit->SetOrigin(Origins::BC);
            //exit->SetPos(obj.position);
            //objList[LayerType::Object][0].push_back(exit);
        }
    }

    for (auto& enemy : enemies)
    {
        enemy->Init(player);
    }


    auto& tiles = objList[LayerType::Tile][0];
    mapSize.left = 0;
    mapSize.top = 0;
    mapSize.width = 1920*4;
    mapSize.height = 1080*4;

    //view sight
    //light.setRange(700.f);
    //fog.setAreaColor(Color(0, 0, 0, 245));

    uiMgr = new GameSceneUiMgr(this);
    uiMgr->Init();

    sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);

    treeMap.setFont(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"));
    treeMap.insert(objList[LayerType::Object][0]);
}

void GameScene::Release()
{
    Scene::Release();
    enemies.clear();
    blockPool.clear();
}

void GameScene::Enter()
{
    enemies.clear();

    Init();

    SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
    SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });

    SOUND_MGR->StopAll();
}

void GameScene::Exit()
{
    treeMap.clear();
    Release();
}

void GameScene::Update(float dt)
{
    for (auto it = enemies.begin(); it != enemies.end(); )
    {
        if (!(*it)->GetActive())
        {
            it = enemies.erase(it);
        }
        else
            it++;
    }

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

    if (player->GetIsMove())
        worldView.setCenter(realcam);

    //view sight pos
    //light.setPosition(player->GetPos());
    //fog.setPosition({ player->GetPos().x - 1920 , player->GetPos().y - 1080  });
    //castAllLights();   

    //mission
    if (Utils::Distance(player->GetPos(), escapePoint) < 100.f)
    {
        escapeTimer -= dt;
        missionText->SetActive(true);
        string timer = to_string(escapeTimer);
        timer = timer.substr(0, timer.find('.') + 3);
        missionText->SetPos({ player->GetPos().x - 300.f,player->GetPos().y - 100.f });
        missionText->SetString("LEAVING IN " + timer);
    }
    else
    {
        missionText->SetActive(false);
        escapeTimer = 3.f;
    }

    if (escapeTimer <= 0.f)
    {
        player->Save();
        SCENE_MGR->ChangeScene(Scenes::Ready);
        return;
    }
    if (!player->GetIsAlive())
    {
        SCENE_MGR->ChangeScene(Scenes::Ready);
        return;
    }

    //treeMap.clear();
    LayerSort();
    //treeMap.~QuadTree();
    //treeMap.insert(objList[LayerType::Object][0]);
    //treeMap.insert(drawObjs);
    treeMap.update(drawObjs);

    //for (auto&& found : treeMap.getObjectsInBound_unchecked(*player)) {
    //   if (player != found && Utils::OBB(player->GetBottom()->GetHitbox(), found->GetBottom()->GetHitbox())) {
    //      found->SetColor(Color::Red);
    //      cout << found->GetName() << endl;
    //   }
    //}
    Scene::Update(dt);
}
vector<HitBoxObject*> GameScene::ObjListObb(HitBoxObject* obj)
{
    return treeMap.getObjectsInBound_unchecked_notParent(*obj);
}
vector<HitBoxObject*> GameScene::ObjListObb(FloatRect obj)
{
    return treeMap.getObjectsInBound_unchecked_notParent(obj);
}


void GameScene::Draw(RenderWindow& window)
{
    window.setView(worldView);
    Vector2i cam_pos = (Vector2i)GetWorldView().getCenter();
    Vector2i cam_size = (Vector2i)GetWorldView().getSize();
    cam_pos = cam_pos - Vector2i{ cam_size.x / 2, cam_size.y / 2 };
    cam_pos /= 60;

    auto view_start_pos = cam_pos + Vector2i{ -1,-1 };
    auto veiw_end_pos = Vector2i{ cam_pos.x + (cam_size.x / 60) + 1 ,cam_pos.y + (cam_size.y / 60) + 1 };

    view_start_pos.x = max(view_start_pos.x, 0);
    view_start_pos.y = max(view_start_pos.y, 0);
    veiw_end_pos.x = min(veiw_end_pos.x, 128);
    veiw_end_pos.y = min(veiw_end_pos.y, 72);

    for (int i = view_start_pos.x; i < veiw_end_pos.x; i++)
    {
        for (int j = view_start_pos.y; j < veiw_end_pos.y; j++)
        {
            if (objList[LayerType::Tile][j][i] != nullptr)
                objList[LayerType::Tile][j][i]->Draw(window);
        }
    }


    //int i = 0;
    //for (auto& obj : objList[LayerType::Tile])
    //{
    //    for (auto& o : obj.second)
    //    {
    //        if (((SpriteObject*)o)->IsInView())
    //            o->Draw(window);
    //    }
    //}
    for (auto& obj : another)
    {
        obj->Draw(window);
    }
    for (auto& obj : drawObjs)
    {
        obj->Draw(window);
    }
    for (auto& obj : objList[LayerType::Object])
    {
        if (obj.first == 0)
            continue;
        for (auto& o : obj.second)
        {
            o->Draw(window);
        }
    }

    //fog.clear();
    //fog.draw(light);
    //window.draw(fog);
    //fog.display();

    if (uiMgr != nullptr)
        uiMgr->Draw(window);
}

void GameScene::SetDeadEnemy(map<string, Item> items, Vector2f pos, Enemy* enemy)
{
    ItemBoxObject* box = new ItemBoxObject();
    box->SetItems(items);
    box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/enemy1-die.png"));

    box->SetOrigin(Origins::MC);
    box->SetHitBox("graphics/enemy1-die.png");
    box->SetPos(pos);
    box->SetName("BOX-ENEMY");
    box->SetPlayerPos(player->GetPosPtr());

    auto boxPos = ((HitBoxObject*)(box))->GetBottomPos() + box->GetGlobalBound().height / 2;

    auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), enemy);
    objList[LayerType::Object][0].insert(it, box);
    sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);
}

void GameScene::DropItems(map<string, Item> items, Vector2f pos)
{
    ItemBoxObject* box = new ItemBoxObject();
    box->SetItems(items);
    box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/box.png"));

    box->SetOrigin(Origins::BC);
    box->SetHitBox("graphics/items/box.png");
    box->SetPos(pos);
    box->SetName("BOX");
    box->SetPlayerPos(player->GetPosPtr());

    auto boxPos = ((HitBoxObject*)(box))->GetBottomPos() + box->GetGlobalBound().height / 2;

    auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), player);
    objList[LayerType::Object][0].insert(it, box);
    sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);
}

void GameScene::EmpytyInven(ItemBoxObject* inven)
{
    if (inven == nullptr)
        return;

    if (inven->GetName() == "BOX")
    {
        deleteContainer.push_back(inven);
        treeMap.remove(inven);
        inven->SetActive(false);
    }
}

candle::EdgeVector GameScene::pushBlock(const sf::Vector2f& pos)
{
    candle::EdgeVector edge;
    const sf::Vector2f points[] = {
          {pos.x - 30, pos.y - 75.f} ,
          {pos.x + 30, pos.y - 75.f} ,
          {pos.x + 30, pos.y } ,
          {pos.x - 30, pos.y } ,
    };
    sfu::Polygon p(points, 4);
    for (auto& l : p.lines)
    {
        edge.push_back(l);
    }
    return edge;
}

void GameScene::castAllLights()
{
    for (auto& it : blockPool)
    {
        //light.castLight(it.edgePool.begin(), it.edgePool.end());

    }
}