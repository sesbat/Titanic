#pragma once
#include "HitBoxObject.h"

class Player;
class Button;
class InventoryBox;
class Inventory;
class Scene;
class Ment;
enum class NPCType
{
    Start,Shop,Craft,Count,
};
class NPC : public HitBoxObject
{
private:
    Player* player;
    NPCType type;

    bool isShowMap;
    bool isShowCraft;
    bool isShowShop;

    Scene* scene;

    Ment* ment;
public:
    NPC();
    virtual ~NPC();

    virtual void Init();
    virtual void Release();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    bool GetShowMap() { return isShowMap; }
    bool GetShowCraft() { return isShowCraft; }
    bool GetShowShop() { return isShowShop; }
    void SetPlayer(Player* player);
    void SetNPCType(NPCType type);
};

