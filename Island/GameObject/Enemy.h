#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"

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

protected:
	Player* player;
	Scene* scene;
	Gun* gun;

	Animator animator;

	RectangleShape healthBar;

	States currState;
	States prevState;

	float speed;
	Vector2f look;
	Vector2f lookDir;
	Vector2f prevLook;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f dir;
	Vector2f prevPosition;
	bool isFlip;

	float moveTime;
	float hitTime;
	float getAttackTime;

	bool attack;

	int maxHp;
	int hp;
	float barScaleX;

	map<string, Item> items;

public:
	Enemy();
	virtual ~Enemy();

	void Init(Player* player);

	void SetState(States newState);
	States GetState();

	void Update(float dt);
	void Draw(RenderWindow& window);

	void OnCompleteDead();

	bool EqualFloat(float a, float b);

	void SetHp(int num);
	void SetHpBar();

	void SetEnemyPos();
	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void Move(float dt);
	void Collision();
	void SetItem(map<string, Item> items) { this->items = items; }
};
