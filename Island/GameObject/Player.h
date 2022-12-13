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

enum class GunType;
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
	Player(const Player& ref) {}
	Player& operator=(const Player& ref) {}
	Player( Player& ref) {}
	Player& operator=( Player& ref) {}
	Scene* scene;
	Gun* gun;
	Inventory* inven;
	ItemBoxObject* rightInvenObj;

	Animator animator;
	
	States currState;
	
	
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

	//valancing variable
	int maxHp;

	float speed;
	float dashSpeed;
	float initSpeed;

	float maxStamina;

	float maxHungerGuage;
	float maxThirstGuage;
	float maxEnergyGuage;
	float maxRadiation;

	float init_hungerDelay;
	float init_thirstDelay;
	float init_energyDelay;
	float init_radiationDelay;

	float hungerDelay;	//delay for hunger decreasion
	float thirstDelay;	//delay for thirst decreasion
	float energyDelay;	//delay for energy decreasion
	float radiationDelay;

	float radDebuffLevel;
	float radDebuffScale;
	float radDebuffHPDelay;

	float initRadBufferScale;

	//float reloadDelay;	//delay for reload
	GunStat gunStat;
	float soundDelay;	//delay for footstep SE

	int magazine;
	//save data variable
	int hp;

	float stamina;
	float hungerGuage;
	float thirstGuage;
	float energyGuage;
	float radGuage;

	int prevHungerGuage;
	int prevThirstGuage;
	int prevEnergyGuage;
	int prevRadGuage;

	int money;
	
	//system variable
	float shootDelay;
	float reloadDelay;

	std::map<GunType, float> shootDelayTime;
	std::map<GunType, float> reloadDelayTime;
	std::map<GunType, int> maxAmmo;
	std::map<GunType, int> ammo;
	std::map<GunType, string> gunBulletName;
	int nowAmmo;

	bool isDash;
	bool isMove;
	bool isAlive;
	bool isReloading;
	bool isInven;
	int lastWephon;

	float staminaDownSpeed ;
	float staminaUpSpeed ;
	
	bool isRad;
	float rad;

	bool isHide;
	float hideDelay;
	float hideDelayTimer;
	bool isHitBullet;
	bool isStun;
	//float stunTime;
	float stun;

	vector<string> clearMaps;
public:
	Player();
	virtual ~Player();
	
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
	Vector2f GetLook() { return look; }
	Vector2f GetPrevLookDir() { return prevLook; }
	Vector2f GetPlayerBottom() { return bottomPos; }
	States GetCurrState() { return currState; }
	bool GetIsDash() { return isDash; }
	bool GetIsMove() { return isMove; }
	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }
	float GetStamina() { return stamina; }
	float GetHungerGuage() { return hungerGuage; }
	float GetThirstGuage() { return thirstGuage; }
	float GetEnergyGuage() { return energyGuage; }
	float GetRadiGuage() { return radGuage; }
	bool GetIsAlive() { return isAlive; }

	bool Hunger() { return prevHungerGuage == (int)hungerGuage; }
	bool Thirst() { return prevThirstGuage == (int)thirstGuage; }
	bool Energy() { return prevEnergyGuage == (int)energyGuage; }
	bool Radiation() { return prevRadGuage == (int)radGuage; }

	void SetHp(int num);
	void SetRad(float rad);
	void HealHp(int num);
	void HealHunger(float num);
	void HealThirst(float num);
	void HealEnergy(float num);
	void HealRad(float num);

	void SetPlayerPos();
	Vector2f SetLookDir();
	void SetFlipX(bool flip);
	void SetIsDash(bool dash);
	void SetMove(bool move);
	void SetPrevHungerGuage(int hunger);
	void SetPrevThirstGuage(int thirst);
	void SetPrevEnergyGuage(int energy);
	void SetPrevRadGuage(int rad);

	void Move(float dt);
	void RadDistance();
	void Collision();
	Vector2f* GetPosPtr() { return &position; }
	void GetItem(map<string, Item>*& items);
	Inventory* GetInventory() { return inven; }
	void SetRigthInvenBox(ItemBoxObject* inven) { rightInvenObj = inven; }
	Gun* GetGun() { return gun; }
	bool GetIsReloading() { return isReloading; }
	void UseItems(int num);
	void SetStamina(float stamina);
	void SetFireAmmo();
	void SetAmmoType();
	void Reload();
	string GetAmmos();

	void Load();
	void Save();

	void PlayerSetIsInven(bool state);
	bool IsInven() { return isInven; }

	int GetMoney() { return money; }
	void AddMoney(int p);
	void SetMoney(int p);

	virtual void Release();

	bool GetHide();
	void SetHide(bool state);
	void HideUpdate(float dt);
	void HideStop();

	void SetStun(bool stun, float time);
	const vector<string>& GetClearMpas() const { return clearMaps; } 
	void ClearMap(string name);
	void SaveSaveBox();
};

