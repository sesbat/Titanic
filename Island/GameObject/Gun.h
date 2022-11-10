#pragma once
#include "SpriteObject.h"
#include "../Framework/ObjectPool.h"

enum class GunType
{
    Shotgun,
    Rifle,
    Sniper,
    TypeCount,
};

class Player;
class Bullet;
class Gun :
    public SpriteObject
{
private:
    GunType gunType;

    ObjectPool<Bullet> bulletPool;

    float bulletSpeed;
    float range;
    float shootDelay;
    int Damage;
    bool isGunFlip;

    Player* player;
public:
    Gun();
    Gun(GunType type);
    ~Gun();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);
    virtual void Relaese();

    void SetPlayer(Player* player);
    void Fire();
};

