#pragma once
#include "HitBoxObject.h"
#include "Animation/Animator.h"
#include "../Framework/ObjectPool.h"
#include "Candle/RadialLight.hpp"
#include "Candle/LightingArea.hpp"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class VertexArrayObj;
class HitBox;
class Scene;
class Gun;
class Bullet;
class Inventory;
class ItemBoxObject;

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
	Scene* scene;
	Gun* gun;

	Animator animator;
	
	States currState;
	
	float speed;
	float maxSpeed;
	Vector2f look;
	Vector2f lookDir;
	Vector2f prevLook;
	Vector2f velocity;
	Vector2f direction;
	Vector2f lastDirection;
	Vector2f prevPosition;
	bool isFlip;
	Vector2f bottomPos;
	Vector2f playerNormalize;

	int hp;
	int maxHp;

	float dash;
	float staminaTime;
	float staminaScale;
	bool isDash;
	float stamina;
	float maxStamina;

	float hungerGuage;
	int prevHungerGuage;
	float thirstGuage;
	int prevThirstGuage;
	float energyGuage;
	int prevEnergyGuage;

	float hungerDelay; 
	float ThirstDelay; 
	float EnergyDelay; 

	bool isMove;

	Inventory* inven;
	ItemBoxObject* rightInvenObj;
	
	bool isAlive;

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
	Vector2f GetPlayerBottom() { return bottomPos; }
	States GetCurrState() { return currState; }
	float GetStaminaScale() { return staminaScale; }
	bool GetIsDash() { return isDash; }
	bool GetIsMove() { return isMove; }
	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }
	float GetStamina() { return stamina; }
	float GetHungerGuage() { return hungerGuage; }
	float GetThirstGuage() { return thirstGuage; }
	float GetEnergyGuage() { return energyGuage; }
	bool GetIsAlive() { return isAlive; }

	bool Hunger() { return prevHungerGuage == (int)hungerGuage; }
	bool Thirst() { return prevThirstGuage == (int)thirstGuage; }
	bool Energy() { return prevEnergyGuage == (int)energyGuage; }

	void SetHp(int num);

	void SetPlayerPos();
	Vector2f SetLookDir();
	void SetFlipX(bool flip);
	void SetIsDash(bool dash);
	void SetMove(bool move);
	void SetPrevHungerGuage(int hunger);
	void SetPrevThirstGuage(int thirst);
	void SetPrevEnergyGuage(int energy);

	void Move(float dt);
	void Collision();
	Vector2f* GetPosPtr() { return &position; }
	void GetItem(map<string, Item>* items);
	Inventory* GetInventory() { return inven; }
	void SetRigthInvenBox(ItemBoxObject* inven) { rightInvenObj = inven; }
	Gun* GetGun() { return gun; }
};

