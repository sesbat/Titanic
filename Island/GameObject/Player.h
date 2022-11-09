#pragma once
#include "SpriteObject.h"
#include "Animation/Animator.h"
#include <list>

using namespace std;

class VertexArrayObj;
class Scene;

class Player : public SpriteObject
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
	//RectangleShape healthBar;

	States currState;
	
	float speed;
	Vector2f look;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f prevPosition;

	SpriteObject* background;
	
	//float timer;
	//float attackTime;

	int hp;
	int maxHp;

	//bool isHitBox;
	//bool isAlive;
public:
	Player();
	virtual~Player();
	
	void Init();

	void SetState(States newState);
	void SetBackground(SpriteObject* bk);

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
	//void SetHpBar();
	//void OnPickupItem(Item* item);
	void SetPlayerPos();
	Vector2f SetLookDir();
};

