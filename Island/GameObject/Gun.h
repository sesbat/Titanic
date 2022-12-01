#pragma once
#include "SpriteObject.h"
#include "../Framework/ObjectPool.h"

enum class GunType
{
    None,
    Shotgun,
    Rifle,
    Sniper,
    TypeCount,
};

enum class User
{
    Player,
    Enemy,
};

class Player;
class Bullet;
class Enemy;

class Gun :
    public SpriteObject
{
private:
    Player* player;
    Enemy* enemy;

    GunType gunType;
    User user;

    ObjectPool<Bullet> bulletPool;

    float bulletSpeed;
    float range;
    float shootDelay;
    int Damage;
    bool isGunFlip;

    Vector2f lookDir;
    Vector2f pos;

    map<string, GunType> gunNameType;
public:
    Gun();
    Gun(GunType type, User user);
    virtual ~Gun();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);
    virtual void Relaese();

    void SetPlayer(Player* player);
    void SetEnemy(Enemy* enemy);
    void Fire(Vector2f pos, bool isplayer);

    void SetGunType(GunType type);
    GunType GetgunType() { return gunType; }
    GunType ItemNameToType(string name);
};

