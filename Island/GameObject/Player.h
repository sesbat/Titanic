#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"
#include "../Framework/ObjectPool.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class VertexArrayObj;
class Gun;
class Scene;
class HitBox;
class Bullet;

class Player : public HitBoxObject
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
	};
protected:
	Animator animator;

	Scene* scene;

	RectangleShape healthBar;

	States currState;
	
	float speed;
	Vector2f look;
	Vector2f lookDir;
	Vector2f prevLook;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f prevPosition;
	bool isFlip;

	Vector2f playerNormalize;

	Gun* shotgun;
	Gun* rifle;
	Gun* sniper;

	SpriteObject* background;

	//float timer;
	//float attackTime;

	int hp;
	int maxHp;
	bool isDash;
	float stamina;

	//bool isAlive;
public:
	Player();
	virtual~Player();
	
	void Init();

	void SetState(States newState);

	void Update(float dt);
	void Draw(RenderWindow& window);

	void Dash(float dt);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	
	bool EqualFloat(float a, float b);

	Vector2f GetPlayerDir() { return direction; }
	Vector2f GetPlayerLastDir() { return lastDirection; }
	Vector2f GetLookDir() { return lookDir; }
	Vector2f GetPrevLookDir() { return prevLook; }
	//int GetDamage() { return damage; }
	States GetCurrState() { return currState; }
	float GetStamina() { return stamina; }
	bool GetIsDash() { return isDash; }

	void SetHp(int num);
	void SetHpBar();
	//void OnPickupItem(Item* item);
	void SetPlayerPos();
	Vector2f SetLookDir();
	void SetFlipX(bool flip);
	void SetIsDash(bool dash);

	//void Fire();
};

