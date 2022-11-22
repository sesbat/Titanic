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
	Vector2f lastPlayerPos;

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

	void SetEnemyPos();
	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }

	void AttackPattern(float dt);
	void Move(float dt);
	void MoveToPos(float dt);
	void Collision();
};
