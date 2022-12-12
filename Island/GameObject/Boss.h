#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

class VertexArrayObj;
class HitBox;
class Player;
class Scene;
class Gun;
class Astar;

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
	enum class Type
	{
		None = -1,
		Big,
		Kiba,
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
	Type type;

	//valance variable
	int maxHp;
	float speed;
	float maxSpeed;
	float dashRange;
	float dashSpeed;
	float stunTime;
	float playerStunTime;
	float activeDashRange;
	float dashCoolTime;
	float dashDamage;
	float damage;
	float radDamage;
	float fireRange;
	float stopTime;
	float moveTime;
	float hitTime;

	//in class variable
	int hp;
	bool isStun;
	bool isStart;
	bool isDash;
	bool isHit;
	bool isInSight;
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
	void Move(float dt);
	void Dash(float dt);
	void Collision();
	void DashCollision();
	void ContactDamage();
	void SetDashPos();
	void Stun();
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void FindGrid(Vector2f pos);
	void CheckIsInWall();
	void CheckIsInSight();
	
};

