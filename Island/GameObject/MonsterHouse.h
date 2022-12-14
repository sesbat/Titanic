#pragma once
#include "Enemy.h"
class MonsterHouse : public Enemy
{
private:
	float spawnTimer;
	float nowTime;
	bool isfindPlayer;
	bool isdead = false;

public:
	MonsterHouse();
	virtual ~MonsterHouse();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Release();
	virtual void Init(Player* player);
	virtual void HideStop() {};
	virtual void SetHp(int num);

};

