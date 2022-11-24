#pragma once
#include "HitBoxObject.h"

class Player;
class Button;
class InventoryBox;
class Inventory;
class Scene;
enum class NPCType
{
    Start,Shop,Count,
};
class NPC : public HitBoxObject
{
private:
    Player* player;
    bool isShowMap;
    NPCType type;

    Inventory* NPCInven;
    InventoryBox* shop;
    Scene* scene;
public:
    NPC();
    virtual ~NPC();

    virtual void Init();
    virtual void Release();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    bool GetShowMap() { return isShowMap; }
    void SetPlayer(Player* player);
    void SetNPCType(NPCType type);
};

