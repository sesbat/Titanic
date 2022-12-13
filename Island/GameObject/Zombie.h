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
class Astar;

class Zombie : public HitBoxObject
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
	Zombie(const Zombie& ref) {}
	Zombie& operator=(const Zombie& ref) {}
	Zombie(Zombie& ref) {}
	Zombie& operator=(Zombie& ref) {}

	Player* player;
	Scene* scene;
	Astar* astar;
	HitBox* dashHitbox;

	Animator animator;

	RectangleShape healthBar;

	States currState;

	//valance variable/////
	int maxHp;
	float maxSpeed;
	float dashSpeed;
	float damage;
	////////////////////////

	float stopTime;
	float moveTime;
	float hitTime;

	float randomNum;
	Vector2f randDir;

	//in class variable
	int hp;
	float speed;
	bool isHit;
	bool isInSight;
	float timer;
	float hitTimer;
	float barScaleX;

	Vector2f look;
	Vector2f lookDir;
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
	Zombie();
	virtual ~Zombie();
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
	void SetZombiePos();

	//void SetType(int num) { type = num; }

	Vector2f GetLookDir() { return lookDir; }


	void AttackPattern(float dt);
	void Move(float dt);
	void Collision();
	void ContactDamage();
	void SetDashPos();
	void SetGreedObject(vector<vector<bool>>* greed) { isGreedObject = greed; }
	void FindGrid();
	void FindGrid(Vector2f pos);
	void CheckIsInSight();
};

