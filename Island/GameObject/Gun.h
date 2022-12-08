#pragma once
#include "SpriteObject.h"
#include "HitBoxObject.h"
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
class HitBox;
class Scene;

class Gun :
    public HitBoxObject
{
private:
    Gun(const Gun& ref) {}
    Gun& operator=(const Gun& ref) {}
    Gun(Gun& ref) {}
    Gun& operator=( Gun& ref) {}
    Player* player;
    Enemy* enemy;
    Scene* scene;

    GunType gunType;
    User user;

    HitBox *gunhitbox;

    ObjectPool<Bullet> bulletPool;

    float bulletSpeed;
    float range;
    float shootDelay;
    float pShootDelay;
    int Damage;
    bool isGunFlip;
    bool isInWall;

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
    float GetpShootDelay() { return pShootDelay; }
    const RectangleShape& GetHitbox() const;
    void SetIsInWall(bool wall) { isInWall = wall; }
    bool GetIsInWall() { return isInWall; }
    void SetLookDir(Vector2f dir) { lookDir = dir; }
};

