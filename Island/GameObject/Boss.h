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

	Animator animator;

	RectangleShape healthBar;

	States currState;
	Type type;

	//valance variable
	float speed;
	float maxSpeed;
	float dashRange;
	float dashSpeed;
	float stunTime;
	float playerStunTime;
	float range;
	float damage;
	float radDamage;
	float fireRange;
	float stopTime;
	float moveTime;

	bool isStun;
	bool isStart;
	bool isDash;
	float timer;


	Vector2f look;
	Vector2f lookDir;
	Vector2f prevLook;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f moveDir;
	Vector2f prevPosition;
	bool isFlip;

	//a star dir
	Vector2f playerPos;
	list<Vector2f> movePos;
	Pair startPos;
	Pair destPos;
	Vector2f bottomPos;
	//dash
	
	//timer
	
	float hitTime;
	float getAttackTime;


	//attack
	bool attack;
	bool isHit;
	bool isSearch;
	//hp
	int maxHp;
	int hp;
	float barScaleX;

	map<string, Item> items;
	vector<vector<bool>>* isGreedObject;

	//init
	
	//dev
	bool isInSight;

	bool isHide;
	float hideDelay;
	float hideDelayTimer;
	bool isHitBullet;

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
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void Move(float dt);
	void Dash(float dt);
	void Collision();
	void SetDashPos();
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void FindGrid(Vector2f pos);
	void CheckIsInWall();
	void CheckIsInSight();
	void MakePath();
	bool CheckWall(int x, int y);
	//void SetIsSearch(bool hit);
	//void CallFriends();
	/*bool GetHide();
	void SetHide(bool state);
	void HideUpdate(float dt);
	void HideStop();*/
};

