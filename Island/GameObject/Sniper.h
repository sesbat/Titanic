#pragma once
#include "SpriteObject.h"

class Player;
class Sniper :
    public SpriteObject
{
private:
    Player* player;
public:
    void SetPlayer(Player* player);

    virtual void Init();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    void Fire();
};

