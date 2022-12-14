#pragma once
#include "Enemy.h"

class Zombie : public Enemy
{
public:
	enum class States
	{
		None = -1,
		Idle,
		Move,
		Dead,
	};
private:
	//Astar* astar;
	//valance variable
	States curState;

	float maxSpeed;
	float dashSpeed;

	bool isdead = false;

public:
	Zombie();
	virtual ~Zombie();
	virtual void Init(Player* player);
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Release();
	virtual void SetState(States newState);
	virtual States GetState();

	virtual void Move(float dt);
	virtual void AttackPattern(float dt);
	virtual void PatrolPattern(float dt);

	virtual void HideStop() {};
	//virtual void SetHp(int num);
	virtual void SetIsSearch(bool hit) {}

};

