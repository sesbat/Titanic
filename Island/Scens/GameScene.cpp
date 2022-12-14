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
#include "../GameObject/MonsterHouse.h"
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
#include "../GameObject/Gun.h"
#include "../GameObject/SupplyBox.h"

#include "../3rd/QuadTree_SFML_DEMO.h"
#include "../GameObject/HitBox.h"
#include "../GameObject/Boss.h"
#include "../GameObject/Ment.h"
#include "../GameObject/ToolTip.h"

using namespace std;
using namespace sf;


bool Lineline(Vector2f bulletpos, Vector2f bulletPrevPos, float x3, float y3, float x4, float y4)
{
    // calculate the direction of the lines
    float uA = ((x4 - x3) * (bulletpos.y - y3) - (y4 - y3) * (bulletpos.x - x3)) /
        ((y4 - y3) * (bulletPrevPos.x - bulletpos.x) - (x4 - x3) * (bulletPrevPos.y - bulletpos.y));
    float uB = ((bulletPrevPos.x - bulletpos.x) * (bulletpos.y - y3) - (bulletPrevPos.y - bulletpos.y) * (bulletpos.x - x3)) /
        ((y4 - y3) * (bulletPrevPos.x - bulletpos.x) - (x4 - x3) * (bulletPrevPos.y - bulletpos.y));

    // if uA and uB are between 0-1, lines are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
    {
        return true;
    }
    return false;
}

bool LineRect(Vector2f bulletpos, Vector2f bulletPrevPos, RectangleShape hitObject)
{
    // check if the line has hit any of the rectangle's sides
    // uses the Line/Line function below
    auto bounds = hitObject.getGlobalBounds();
    float rx = bounds.left;
    float ry = bounds.top;
    float rw = bounds.width;
    float rh = bounds.height;
    bool left = Lineline(bulletpos, bulletPrevPos, rx, ry, rx, ry + rh);
    bool right = Lineline(bulletpos, bulletPrevPos, rx + rw, ry, rx + rw, ry + rh);
    bool top = Lineline(bulletpos, bulletPrevPos, rx, ry, rx + rw, ry);
    bool bottom = Lineline(bulletpos, bulletPrevPos, rx, ry + rh, rx + rw, ry + rh);

    // if ANY of the above are true, the line
    // has hit the rectangle
    if (left || right || top || bottom) {
        return true;
    }
    return false;
}
//explicit QuadTree(sf::FloatRect bounds,   size_t maxLevel, size_t maxObjects);
GameScene::GameScene()
    :Scene(Scenes::GameScene), timer(0.f), escapeTimer(3.f),
       fog(candle::LightingArea::FOG,
          sf::Vector2f(0.f, 0.f),
          sf::Vector2f(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2)),
        blockCount(0), treeMap(treeRect, 16, 4), lines(LineStrip, 2),isZoom(false),
    supplyTimer(0), initSupplyTimer(60), isSupply(false)
{
    FRAMEWORK->GetWindow().setMouseCursorVisible(false);
}

GameScene::~GameScene()
{
    Release();
}


void GameScene::Init()
{
    uiMgr = new GameSceneUiMgr(this);
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

    auto enemyInfo = FILE_MGR->GetEnemyInfo();

    for (auto& obj : data)
    {
        if (obj.type == "STONE" || obj.type == "BLOCK" || obj.type == "RADIATION"|| obj.type == "INVISIBLE")
        {
            isGreedObject[obj.greedIdx.x][obj.greedIdx.y] = true;
            Blocks block = { obj.position, pushBlock(obj.position) };
            blockPool.push_back(block);
            //blockCount++;
        }
        if (obj.type == "BOX" || obj.type == "BOX-ENEMY")
        {
            ItemBoxObject* box = new ItemBoxObject();
            box->SetPos(obj.position);
            box->Init();
            box->SetPlayerPos(player->GetPosPtr());
            box->SetItems(obj.item);
            box->SetTexture(*RESOURCES_MGR->GetTexture("graphics/items/box.png"));
            box->SetName(obj.type);
            box->SetOrigin(Origins::BC);
            box->SetHitBox(obj.path);
            box->SetPlayer(player);
            objList[LayerType::Object][0].push_back(box);
        }
        else if (obj.type == "TREE" || obj.type == "BUSH" ||
            obj.type == "STONE" || obj.type == "BLOCK" || obj.type == "RADIATION"|| obj.type == "INVISIBLE" || obj.type == "RADTILE")
        {
            HitBoxObject* draw = new HitBoxObject();
            draw->SetName(obj.type);
            draw->SetTexture(*RESOURCES_MGR->GetTexture(obj.path));
            draw->SetOrigin(Origins::BC);
            draw->SetPos(obj.position);
            draw->SetHitBox(obj.path);
            if (obj.type == "INVISIBLE" || obj.type == "RADTILE")
            {
                draw->SetColor(Color (0, 0, 0, 0));
            }

            objList[LayerType::Object][0].push_back(draw);
        }
        else if (obj.type == "PLAYER")
        {
            player->SetPos(obj.position);
            player->SetHitBox(obj.path);

            objList[LayerType::Object][0].push_back(player);
        }
        else if (obj.type == "BOSS")
        {
            Boss* boss = new Boss();
            boss->SetName(obj.type);
            boss->SetId(enemyId++);
            boss->SetPos(obj.position);
            boss->SetHitBox(obj.path);
            //boss->SetType((GunType)enemyInfo[obj.path].gun);
            //boss->SetEnemyType(enemyInfo[obj.path].type);
            boss->SetItem(obj.item);
            boss->SetGreedObject(&isGreedObject);
            objList[LayerType::Object][0].push_back(boss);
            bosses.push_back(boss);
        }
        else if (obj.type == "ENEMY")
        {
            if (obj.path == "graphics/enemy/s_infestation.png")
            {
                MonsterHouse* enemy = new MonsterHouse();
                enemy->SetName(obj.type);
                enemy->SetId(enemyId++);
                enemy->SetPos(obj.position);
                enemy->SetHitBox(obj.path);
                enemy->SetEnemyType(enemyInfo[obj.path].type);
                enemy->SetItem(obj.item);
                enemy->SetGreedObject(&isGreedObject);
                enemies.push_back(enemy);
                objList[LayerType::Object][0].push_back(enemy);
			}
			else
			{
				Enemy* enemy = new Enemy();
				enemy->SetName(obj.type);
				enemy->SetId(enemyId++);
				enemy->SetPos(obj.position);
				enemy->SetHitBox(obj.path);
				enemy->SetType((GunType)enemyInfo[obj.path].gun);
				enemy->SetEnemyType(enemyInfo[obj.path].type);
				enemy->SetItem(obj.item);
				enemy->SetGreedObject(&isGreedObject);
				enemies.push_back(enemy);

				objList[LayerType::Object][0].push_back(enemy);
			}

        }
        else if (obj.type == "TILE" )
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
        }
    }

    for (auto& enemy : enemies)
    {
        enemy->Init(player);
    }
    for (auto& boss : bosses)
    {
        boss->Init(player);
    }
    //boss
    /*if (testboss)
    {
        boss->Init(player);
    }*/

    //

    auto& tiles = objList[LayerType::Tile][0];
    mapSize.left = 0;
    mapSize.top = 0;
    mapSize.width = 1920 * 4;
    mapSize.height = 1080 * 4;

    //view sight
    light.setRange(700.f);
    fog.setAreaColor(Color(0, 0, 0, 245));

    uiMgr->Init();

    sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);

    treeMap.setFont(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"));
    treeMap.insert(objList[LayerType::Object][0]);

    SOUND_MGR->Play("sounds/gameBGM.ogg", true);

    cursor = new SpriteObject();
    cursor->SetTexture(*RESOURCES_MGR->GetTexture("graphics/cursor.png"));
    cursor->SetOrigin(Origins::MC);
    cursor->SetUI(true);

    shot_cursor = new SpriteObject();
    shot_cursor->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shot_cursor.png"));
    shot_cursor->SetOrigin(Origins::MC);
    shot_cursor->SetUI(true);

    objList[LayerType::Cursor][0].push_back(cursor);
    objList[LayerType::Cursor][1].push_back(shot_cursor);
}

void GameScene::Release()
{
    treeMap.clear();
    Scene::Release();
    enemies.clear();
    bosses.clear();
    blockPool.clear();
    bosses.clear();
}

void GameScene::Enter()
{
    enemies.clear();
    radObj.clear();

    Init();

    SCENE_MGR->GetCurrScene()->GetWorldView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    SCENE_MGR->GetCurrScene()->GetWorldView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });
    SCENE_MGR->GetCurrScene()->GetUiView().setCenter({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    SCENE_MGR->GetCurrScene()->GetUiView().setSize({ WINDOW_WIDTH , WINDOW_HEIGHT });

    
}

void GameScene::Exit()
{
    SOUND_MGR->StopAll();
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

    for (auto it = bosses.begin(); it != bosses.end(); )
    {
        if (!(*it)->GetActive())
        {
            it = bosses.erase(it);
        }
        else
            it++;
    }

    Vector2f mouseworldPos = FRAMEWORK->GetWindow().mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), worldView);


    lines[0].position = player->GetPos();
    lines[1].position = mouseworldPos;

    Vector2f dir;
    dir.x = mouseworldPos.x - player->GetPos().x;
    dir.y = mouseworldPos.y - player->GetPos().y;

    //camera
    
    if (InputMgr::GetMouseButtonDown(Mouse::Right))
    {
        isZoom = true;
    }
    if (InputMgr::GetMouseButtonUp(Mouse::Right))
    {
        isZoom = false;
    }
    
    if (isZoom)
    {
        if (r <= MaxR)
            r += (MaxR - r)* zoomInOutSpeed;
    }
    else
    {
        if (r >= MinR)
            r -= (MinR + r) * zoomInOutSpeed;
    }

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
    SupplyUpdate(dt);

    //view sight pos
    light.setPosition(player->GetPos());
    fog.setPosition({ player->GetPos().x - 1920 , player->GetPos().y - 1080  });

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
        player->ClearMap(sceneName);
        player->Save();
        SCENE_MGR->ChangeScene(Scenes::Ready);
        return;
    }
    if (!player->GetIsAlive())
    {
        player->SaveSaveBox();
        FILE_MGR->SaveInvenInfo(vector<InvenInfo>());
        FILE_MGR->SaveUseItemInfo(vector<InvneUseInfo>());
        SCENE_MGR->ChangeScene(Scenes::Ready);
        return;
    }

    LayerSort();
    treeMap.update(drawObjs);

    Scene::Update(dt);
    if (InputMgr::GetKeyDown(Keyboard::Escape))
    {
        SCENE_MGR->ChangeScene(Scenes::Ready);
        return;
    }

    vector<HitBoxObject*> boundInObj;
   
    boundInObj = ObjListObb(lines.getBounds());

    shot_cursor->SetColor(Color::White);
    bool nowTargeting = false;
    for (auto& obj : boundInObj)
    {
        if (obj->GetName() == "TREE" ||
            obj->GetName() == "STONE" ||
            obj->GetName() == "BLOCK" ||
            obj->GetName() == "RADIATION"||
            obj->GetName() == "INVISIBLE")
        {
            auto hit = obj->GetBottom();
            if (LineRect(
                lines[0].position,
                lines[1].position,
                hit->GetHitbox()))
            {
                    shot_cursor->SetColor(Color::Red);
                    targeting = true;
                    nowTargeting = true;
                break;
            }
        }
    }

    cursor->SetPos(lines[1].position);
    shot_cursor->SetPos(lines[1].position);

    //boss->Update(dt);
}

void GameScene::SupplyUpdate(float dt)
{

    if (!isSupply)
    {
        supplyTimer += dt;
        if (supplyTimer > initSupplyTimer)
        {
            SupplyBox* supBox = new SupplyBox();
            supBox->SetTexture(*RESOURCES_MGR->GetTexture("graphics/enemy1-die.png"));
            supBox->SetOrigin(Origins::MC);
            supBox->SetHitBox("graphics/enemy1-die.png");
            supBox->SetName("SupplyBox");
            supBox->SetPlayerPos(player->GetPosPtr());
            supBox->SetPlayer(player);
            supBox->SetPos(player->GetPos());
            supBox->Init();
            supBox->SetRandPos();

            auto supplyBoxData = FILE_MGR->GetSupplyBoxInfo(sceneName);
            supBox->SetSupplyItems(supplyBoxData);

            objList[LayerType::Object][0].push_back(supBox);
            sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);

            Ment* ment = new Ment();
            ment->SetUiViewCenter(true);
            ment->SetUiView(&uiView);
            ment->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 24, Color::White, "The supply box has appeared. ");
            ment->SetOrigin(Origins::MC);
            ment->SetTimer(2);
            ment->SetAlways(false);
            ment->SetActive(true);
            objList[LayerType::Object][1].push_back(ment);


            isSupply = true;
        }
    }
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

	window.setView(worldView);

	window.draw(lines);
	if (!player->GetIsMove())
		cursor->Draw(window);
	if (player->GetIsMove())
		shot_cursor->Draw(window);

   //boss->Draw(window);
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
    box->SetPlayer(player);
    box->Init();

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
    box->SetPlayer(player);
    box->Init();

    auto boxPos = ((HitBoxObject*)(box))->GetBottomPos() + box->GetGlobalBound().height / 2;

    auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), player);
    objList[LayerType::Object][0].insert(it, box);
    sort(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), sorting);
}

void GameScene::EmpytyInven(ItemBoxObject* inven)
{
    if (inven == nullptr)
        return;

    if (inven->GetName() == "BOX" || inven->GetName() == "SupplyBox")
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

void GameScene::CloseToolTip()
{
    ((GameSceneUiMgr*)uiMgr)->GetTip()->SetActive(false);
}

