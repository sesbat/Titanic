#pragma once
#include "SpriteObject.h"
#include "Animation/Animator.h"
#include "../Framework/ObjectPool.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class VertexArrayObj;
class Scene;
class HitBox;
class Bullet;
class Player : public SpriteObject
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
	};
	enum class GunType
	{
		Shotgun,
		Rifle,
		Sniper,
		GunCount,
	};
protected:
	Animator animator;

	Scene* scene;

	HitBox* bottomHitBox;
	vector<HitBox*> bodyHitBox;

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

	ObjectPool<Bullet> bulletPool;

	Vector2f playerNormalize;

	//SpriteObject* background;

	GunType gun;

	SpriteObject* shotgun;
	SpriteObject* background;
	
	//float timer;
	//float attackTime;

	int hp;
	int maxHp;

	bool isHitBox;
	//bool isAlive;
public:
	Player();
	virtual~Player();
	
	void Init();

	void SetState(States newState);
	//void SetBackground(SpriteObject* bk);

	void Update(float dt);
	void Draw(RenderWindow& window);

	void Dash(float dt);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	
	bool EqualFloat(float a, float b);

	Vector2f GetPlayerDir() { return direction; }
	Vector2f GetPlayerLastDir() { return lastDirection; }
	//int GetDamage() { return damage; }
	States GetCurrState() { return currState; }

	void SetHp(int num);
	void SetHpBar();
	//void OnPickupItem(Item* item);
	void SetPlayerPos();
	Vector2f SetLookDir();
	void SetFlipX(bool flip);

	void Fire();
};

