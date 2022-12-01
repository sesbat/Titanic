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
	Inventory* inven;
	ItemBoxObject* rightInvenObj;

	Animator animator;
	
	States currState;
	
	float speed;
	const float maxSpeed;
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
	const int maxHp;

	float dash;
	float staminaTime;
	float staminaScale;
	bool isDash;
	float stamina;
	const float maxStamina;

	float hungerGuage;
	int prevHungerGuage;
	const float maxHungerGuage;
	
	float thirstGuage;
	int prevThirstGuage;
	const float maxThirstGuage;
	
	float energyGuage;
	int prevEnergyGuage;
	const float maxEnergyGuage;

	float hungerDelay; 
	float ThirstDelay; 
	float EnergyDelay; 

	//item
	int ammo;
	int sgAmmo;
	int rfAmmo;
	int snAmmo;
	int magazineSG;
	int magazineRF;
	int magazineSN;

	bool isMove;
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
	void HealHp(int num);
	void HealHunger(float num);
	void HealThirst(float num);
	void HealEnergy(float num);

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
	void UseItems(int num);
	void SetAmmo();
	void Reload();

	void Load();
	void Save();
};

