#pragma once
#include "SpriteObject.h"

class Player;
class Button;
class NPC :
    public SpriteObject
{
private:
    Player* player;
    bool isShowMap;
public:
    NPC();
    virtual ~NPC();

    virtual void Init();
    virtual void Release();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    bool GetShowMap() { return isShowMap; }
    void SetPlayer(Player* player);
};

