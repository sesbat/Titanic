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
    Start,Shop,Craft,Heal,Count,
};
class NPC : public HitBoxObject
{
private:
    NPC(const NPC& ref) {}
    NPC& operator=(const NPC& ref) {}
    NPC( NPC& ref) {}
    NPC& operator=( NPC& ref) {}
    Player* player;
    NPCType type;

    bool isShowMap;
    bool isShowCraft;
    bool isShowShop;
    bool isHeal;

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
    bool GetIsHeal() { return isHeal; }

    void SetIsHeal(bool yesORno);
    void SetPlayer(Player* player);
    void SetNPCType(NPCType type);
};

