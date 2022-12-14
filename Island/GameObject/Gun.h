#pragma once
#include "SpriteObject.h"
#include "HitBoxObject.h"
#include "../Framework/ObjectPool.h"

enum class GunType
{
    None,
    Shotgun,
    Up1_ShotGun,
    Rifle,
    Sniper,
    MB_Shotgun,
    MB_Rifle,
    MB_sniper,
    Scop_Rifle,
    Scop_sniper,
    SR_25,
    TypeCount,
};

enum class User
{
    Player,
    Enemy,
    Boss,
};

class Player;
class Bullet;
class Enemy;
class HitBox;
class Scene;
class Boss;
struct GunsType;

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
    Boss* boss;
    Scene* scene;

    GunType gunType;
    User user;

    HitBox *gunhitbox;

    ObjectPool<Bullet> bulletPool;

    float bulletSpeed;
    float range;
    float shootDelay;
    float reloadDelay;
    int damage;
    bool isGunFlip;
    bool isInWall;

    Vector2f lookDir;
    Vector2f pos;
    Vector2f randDir;
    float randomNum;

    map<string, GunType> gunNameType;

    map<string, GunStat> gunsInfo;
    GunStat data;
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
    void SetBoss(Boss* enemy);
    void Fire(Vector2f pos, bool isplayer);
    //void BossFire(Vector2f pos, bool isplayer = false);
    void BossFire(Vector2f pos, bool isplayer = false);

    void SetGunType(GunType type);
    GunType GetgunType() { return gunType; }
    GunType ItemNameToType(string name);
    float GetpShootDelay() { return shootDelay; }
    float GetReloadDelay() { return reloadDelay; }
    const RectangleShape& GetHitbox() const;
    void SetIsInWall(bool wall) { isInWall = wall; }
    bool GetIsInWall() { return isInWall; }
    void SetLookDir(Vector2f dir) { lookDir = dir; }
};

