#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"

class Player;
class VertexArrayObj;
class HitBox;
class Scene;

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

	Animator animator;

	RectangleShape healthBar;

	States currState;

	float speed;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f dir;
	Vector2f prevPosition;

	int bossState;
	float moveTime;
	float hitTime;
	float getAttackTime;

	bool attack;

	int maxHp;
	int hp;
	float barScaleX;

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

	void AttackPattern(float dt);
	void Move(float dt);
	void Collision();
};
