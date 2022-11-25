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

	struct Cell {
		int parentX, parentY;
		double f, g, h;
	};

protected:
	Player* player;
	Scene* scene;
	Gun* gun;

	Animator animator;

	RectangleShape healthBar;

	States currState;
	States prevState;

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

	Vector2f playerPos;
	list<Vector2f> movePos;
	Pair startPos;
	Pair destPos;

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
	vector<vector<bool>> isGreedObject;

	int xDir[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int yDir[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
public:
	Enemy();
	virtual ~Enemy();

	void Init(Player* player, int type = 1);

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

	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void Move(float dt);
	void MoveToPos(float dt);
	void Collision();
	
	void SetGreedObject(vector<vector<bool>> greed) { isGreedObject = greed; }

	bool IsValid(int x, int y);
	bool IsDestination(int x, int y, Pair dest);
	int GetDistance(int x1, int y1, int x2, int y2);
	void TracePath(Cell cellDetails[36][64], Pair dst);
	void FindGrid();
	void AstarSearch(vector<vector<bool>> map, Pair start, Pair dest);
};
