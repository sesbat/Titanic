#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"
#include "../Framework/ObjectPool.h"
#include "SpriteObject.h"

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

class VertexArrayObj;
class HitBox;
class Player;
class Scene;
class Gun;
class Astar;
//class Bullet;

class Boss : public HitBoxObject
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Dash,
		Dead,
	};

protected:
	Boss(const Boss& ref) {}
	Boss& operator=(const Boss& ref) {}
	Boss(Boss& ref) {}
	Boss& operator=(Boss& ref) {}

	Player* player;
	Scene* scene;
	Gun* gun;
	Astar* astar;
	HitBox* dashHitbox;

	Animator animator;

	RectangleShape healthBar;

	States currState;

	//valance variable/////
	int maxHp;
	float maxSpeed;
	float dashRange;
	float dashSpeed;
	float stunTime;
	float playerStunTime;
	float activeDashRange;
	float dashCoolTime;
	float dashDamage;
	int damage;
	int fireDamage;
	float startRange;
	
	//Boss fire
	float radDamage;
	float fireRange;
	float fireSpeed;
	float shootDelay;
	float fireAngle;
	int fireCount;

	//single shot
	float singleRange;
	float singleSpeed;
	float singleshootDelay;
	float singleAngle;
	int singleCount;

	//rampage
	float rampfireRange;
	float rampfireSpeed;
	float rampshootDelay;
	float rampfireAngle;
	int rampfireCount;

	float semiDelay;
	int fpCount;
	////////////////////////

	int rampageCount;
	int firePattern;

	float rd;
	float fr;
	float fs;
	float sd;
	float fa;
	int fc;
	int rampCount;
	
	float stopTime;
	float moveTime;
	float hitTime;

	float randomNum;
	Vector2f randDir;

	//in class variable
	int hp;
	float speed;
	bool isStun;
	bool isStart;
	bool isDash;
	bool isHit;
	bool isInSight;
	bool isInWall;
	bool isFire;
	float timer;
	float range;
	float dashTimer;
	float hitTimer;
	int dashAttack;
	float barScaleX;

	Vector2f look;
	Vector2f lookDir;
	Vector2f dashDir;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f moveDir;
	Vector2f prevPosition;
	Vector2f dashPosition;

	//a star dir
	Vector2f playerPos;
	list<Vector2f> movePos;
	Pair startPos;
	Pair destPos;
	Vector2f bottomPos;
	vector<vector<bool>>* isGreedObject;

	map<string, Item> items;
	

public:
	Boss();
	virtual ~Boss();
	virtual void Release();

	void Init(Player* player);

	void SetState(States newState);
	States GetState();

	void Update(float dt);
	void Draw(RenderWindow& window);

	void OnCompleteDead();

	bool EqualFloat(float a, float b);

	void SetHp(int num);
	void SetHpBar();

	void SetItem(map<string, Item> items) { this->items = items; }
	void SetBossPos();

	//void SetType(int num) { type = num; }

	Vector2f GetLookDir() { return lookDir; }
	

	void AttackPattern(float dt);
	void RampagePattern(float dt);
	void FirePattern(float dt);
	void Move(float dt);
	void Dash(float dt);
	void RampageDash(float dt);
	void Collision();
	void RampCollision();
	void DashCollision();
	void ContactDamage();
	void SetDashPos();
	void SetRampagePos();
	void Stun();
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void FindGrid(Vector2f pos);
	void CheckIsInWall();
	void CheckIsInSight();

	void SetFireVariable();
	void SetRampageVariable();
	void SetSingleVariable();

	float GetFireSpeed() { return fs; }
	float GetFireRange() { return fr; }
	float GetShootDelay() { return sd; }
	int GetFireCount() { return fc; }
	float GetFireAngle() { return fa; }
	float GetRadiation() { return radDamage; }
	int GetFireDamage() { return fireDamage; }
};

