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
enum class GunType;

class Enemy : public HitBoxObject
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Dead,
	};

protected:
	Enemy(const Enemy& ref) {}
	Enemy& operator=(const Enemy& ref) {}
	Enemy( Enemy& ref) {}
	Enemy& operator=( Enemy& ref) {}
	Player* player;
	Scene* scene;
	Gun* gun;
	Astar* astar;

	Animator animator;

	RectangleShape healthBar;

	States currState;

	//move
	float speed;
	Vector2f look;
	Vector2f lookDir;
	Vector2f prevLook;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f moveDir;
	Vector2f prevPosition;
	bool isFlip;
	Vector2f patrolPos;
	int patrolBlock;

	//a star dir
	Vector2f playerPos;
	list<Vector2f> movePos;
	Pair startPos;
	Pair destPos;
	Vector2f bottomPos;
	Vector2f firstPos;
	//timer
	float moveTime;
	float initMoveTime;

	float hitTime;
	float initHitTime;
	
	float patrolTime;
	float initPatrolTime;
	
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
	vector<vector<bool>> *isGreedObject;

	//init
	GunType type;
	string enemyType;
	//dev
	bool isInSight;

	bool isHide;
	float hideDelay;
	float hideDelayTimer;
	bool isHitBullet;
public:
	Enemy();
	virtual ~Enemy();
	virtual void Release();

	virtual void Init(Player* player);

	void SetState(States newState);
	States GetState();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void OnCompleteDead();

	bool EqualFloat(float a, float b);

	virtual	void SetHp(int num);
	void SetHpBar();

	void SetItem(map<string, Item> items) { this->items = items; }
	void SetEnemyPos();

	void SetType(GunType t) { type = t; }
	void SetEnemyType(string t) { enemyType = t; }

	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void PatrolPattern(float dt);
	void Move(float dt);
	void MoveToPos(float dt);
	void Collision();
	
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void FindGrid(Vector2f pos);
	void CheckIsInWall();
	void CheckIsInSight();
	void MakePath();
	bool CheckWall(int x, int y);
	void SetIsSearch(bool hit);
	void CallFriends();
	bool GetHide();
	void SetHide(bool state);
	void HideUpdate(float dt);
	virtual void HideStop();
};
