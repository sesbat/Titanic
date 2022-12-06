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
	
	//a star dir
	Vector2f playerPos;
	list<Vector2f> movePos;
	Pair startPos;
	Pair destPos;
	Vector2f bottomPos;

	//timer
	float moveTime;
	float hitTime;
	float getAttackTime;

	//attack
	bool attack;
	bool isHit;
	
	//hp
	int maxHp;
	int hp;
	float barScaleX;

	map<string, Item> items;
	vector<vector<bool>> *isGreedObject;

	int type;
	//dev
	//bool isMove;

public:
	Enemy();
	virtual ~Enemy();
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
	void SetEnemyPos();

	void SetType(int num) { type = num; }

	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void Move(float dt);
	void MoveToPos(float dt);
	void Collision();
	
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void CheckIsInWall();
};
