#include "Player.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/SoundManager.h"
#include "../Scens/SceneManager.h"
#include "VertexArrayObj.h"
#include "Bullet.h"
#include "Object.h"
#include "HitBox.h"
#include "../Scens/GameScene.h"
#include "../Scens/Ready.h"
#include <iostream>
#include "Gun.h"
#include "../Ui/Inventory.h"
#include "../Ui/InventoryBox.h"
#include "../Ui/InvenItem.h"
#include "../GameObject/ItemBoxObject.h"
#include "../GameObject/TextObject.h"


Player::Player()
	: currState(States::None),
	look(1.f, 0.f), prevLook(1.f, 0.f),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	isDash(false), isAlive(true), isMove(true), shootDelay(0.f),
	isReloading(false), soundDelay(0.5f), isStun(false), stun(0.f)
{
	auto stat = FILE_MGR->GetUserStat();

	dashSpeed = stat.dashSpeed;
	maxHp = stat.maxHp;
	init_energyDelay = energyDelay = stat.energyDelay;
	init_hungerDelay = hungerDelay = stat.hungerDelay;
	init_radiationDelay = radiationDelay = stat.radiationDelay;
	maxEnergyGuage = stat.maxEnergyGuage;
	maxHungerGuage = stat.maxHungerGuage;
	maxRadiation = stat.maxRadiation;
	maxStamina = stat.maxStamina;
	maxThirstGuage = stat.maxThirstGuage;
	radDebuffLevel = stat.radDebuffLevel;
	initRadBufferScale = stat.radDebuffScale;
	radDebuffScale = 1;
	radDebuffHPDelay = stat.radDebuffHPDelay;

	init_radiationDelay = radiationDelay = stat.radiationDelay;
	initSpeed = speed = stat.speed;
	staminaDownSpeed = stat.staminaDownSpeed;
	staminaUpSpeed = stat.staminaUpSpeed;
	init_thirstDelay = thirstDelay = stat.thirstDelay;

	hideDelay = 1.f;
}

Player::~Player()
{
	Release();
}

void Player::Init()
{
	HitBoxObject::Init();

	gun = new Gun(GunType::None, User::Player);
	gun->SetPlayer(this);
	gun->Init();

	animator.SetTarget(&sprite);

	//animation
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerIdle"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerMove"));

	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerIdleLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerMoveLeft"));

	scene = SCENE_MGR->GetCurrScene();

	SetState(States::Idle);

	inven = new Inventory(scene->GetUiMgr());
	inven->Init();
	inven->SetActive(false);

	//light.setRange(300.f);

	Load();
	inven->GetMoneyTxt()->SetString(to_string(money));
	inven->GetMoneyTxt()->SetOrigin(Origins::MR);

}

void Player::SetState(States newState)
{
	currState = newState;
	switch (currState)
	{
	case Player::States::Idle:
		animator.Play((lookDir.x > 0.f) ? "PlayerIdle" : "PlayerIdleLeft");
		break;
	case Player::States::Move:
		animator.Play((lookDir.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection = direction;
		break;
	}
}

void Player::Update(float dt)
{

	HitBoxObject::Update(dt);
	Vector2i mousePos = (Vector2i)InputMgr::GetMousePos();
	Vector2f mouseWorldPos = scene->ScreenToWorld(mousePos);

	look = mouseWorldPos;
	lookDir = Utils::Normalize(mouseWorldPos - GetPos());
	if (!isStun)
	{
		direction.x = InputMgr::GetAxisRaw(Axis::Horizontal);
		direction.y = InputMgr::GetAxisRaw(Axis::Vertical);
	}


	soundDelay -= dt;
	if (soundDelay <= 0 && (InputMgr::GetAxisRaw(Axis::Horizontal) || InputMgr::GetAxisRaw(Axis::Vertical)))
	{
		SOUND_MGR->Play("sounds/footstep.ogg");
		soundDelay = 0.5f;
	}

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	}
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		hungerDelay -= dt;
		thirstDelay -= dt;
		energyDelay -= dt;

		RadDistance();
		if (isRad)
		{
			radiationDelay -= dt;
		}
		if (hungerDelay < 0.f && hungerGuage > 0.f)
		{
			hungerGuage -= 2.5;
			hungerDelay = init_hungerDelay * radDebuffScale;
		}
		if (thirstDelay < 0.f && thirstGuage > 0.f)
		{
			thirstGuage -= 2.5;
			thirstDelay = init_thirstDelay * radDebuffScale;
		}
		if (energyDelay < 0.f && energyGuage > 0.f)
		{
			energyGuage -= 2.5;
			energyDelay = init_energyDelay * radDebuffScale;
		}
		if (radiationDelay < 0.f && radGuage <= 255.f)
		{
			radGuage += 2.5;
			radiationDelay = init_radiationDelay;
			if ((radGuage / 255) * 100 >= radDebuffLevel)
			{
				radDebuffScale = initRadBufferScale;
			}
			else
			{
				radDebuffScale = 1;
			}
		}

	}

	//Dead
	if (hp <= 0 || hungerGuage <= 0 ||
		thirstGuage <= 0 || energyGuage <= 0 || radGuage >= 255)
	{
		isAlive = false;
	}

	//Move
	if (isMove && !isStun)
		Move(dt);

	//animation
	animator.Update(dt);

	if (!EqualFloat(direction.x, 0.f))
	{
		lastDirection = direction;
	}
	bottomPos = bottom->GetHitBottomPos();
	prevLook = lookDir;

	gun->Update(dt);
	//SetAmmoType();
	//input
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		shootDelay -= dt;
		reloadDelay -= dt;

		//fire
		if (!isStun)
		{
			if (reloadDelay <= 0)
			{
				isReloading = false;
				if (ammo[gun->GetgunType()] > 0 && shootDelay <= 0 && gun->GetIsInWall())
				{
					//isReloading = false;
					switch (gun->GetgunType())
					{
					case GunType::Shotgun:
					case GunType::Sniper:
					case GunType::Up1_ShotGun:

						if (InputMgr::GetMouseButtonDown(Mouse::Left) && !inven->GetActive())
						{
							SetFireAmmo();
							gun->Fire(GetPos(), true);
							HideStop();
							shootDelay = gun->GetpShootDelay();
						}
						break;
					case GunType::Rifle:
						if (InputMgr::GetMouseButton(Mouse::Left) && !inven->GetActive())
						{
							SetFireAmmo();
							gun->Fire(GetPos(), true);
							HideStop();
							shootDelay = gun->GetpShootDelay();

						}
						break;
					}
				}
			}

			//reload
			if (InputMgr::GetKeyDown(Keyboard::R))
			{
				Reload();

			}
		}
	}
	if (!isStun)
	{
		if (!isReloading)
		{
			if (InputMgr::GetKeyDown(Keyboard::Num1))
			{
				auto myGun = inven->GetUsedItem(0);

				if (myGun == nullptr)
				{
					gun->SetGunType(GunType::None);
					ammo[gun->GetgunType()] = 0;
				}
				else
				{
					gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
					//SetAmmoType();
				}
				lastWephon = 0;
			}
			if (InputMgr::GetKeyDown(Keyboard::Num2))
			{
				auto myGun = inven->GetUsedItem(1);

				if (myGun == nullptr)
				{
					gun->SetGunType(GunType::None);
					ammo[gun->GetgunType()] = 0;
				}
				else
				{
					gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
					//SetAmmoType();
				}
				lastWephon = 1;
			}
		}

		//use item
		if (InputMgr::GetKeyDown(Keyboard::Num3))
		{
			UseItems(4);
		}
		if (InputMgr::GetKeyDown(Keyboard::Num4))
		{
			UseItems(5);
		}
		if (InputMgr::GetKeyDown(Keyboard::Num5))
		{
			UseItems(6);
		}
		if (InputMgr::GetKeyDown(Keyboard::Num6))
		{
			UseItems(7);
		}

		//dash
		if (stamina > 0.5f && InputMgr::GetKeyDown(Keyboard::LShift))
		{
			speed = dashSpeed;
			isDash = true;
		}
		if (InputMgr::GetKeyUp(Keyboard::LShift))
		{
			speed = initSpeed;
			isDash = false;
		}
	}
	else
	{
		speed = initSpeed;
		isDash = false;
	}
	
	if (InputMgr::GetKeyDown(Keyboard::Tab) || InputMgr::GetKeyDown(Keyboard::Escape))
	{
		if (inven->GetActive() && !isMove)
		{
			InputMgr::Clear();
			SetMove(true);
			inven->SetActive(!(inven->GetActive()));

			//inven->ResetRightInven();
			if (inven->GetRightInven()->GetItems()->size() > 0 && isInven)
			{
				auto rightInven = inven->GetRightInven()->GetItems();
				map<string, Item> dropItems;
				for (auto& item : *rightInven)
				{
					if (dropItems.find(item->GetName()) == dropItems.end())
					{
						dropItems[item->GetName()] = { item->GetName(), item->GetPath(),item->GetCount() };
					}
					else
					{
						dropItems[item->GetName()].count += item->GetCount();
					}
				}
				((GameScene*)scene)->DropItems(dropItems, position);
			}
			if (rightInvenObj != nullptr)
			{
				auto items = rightInvenObj->GetInvenBox()->GetItems();
				if (items->size() == 0)
				{
					((GameScene*)scene)->EmpytyInven(rightInvenObj);
					rightInvenObj = nullptr;
				}
			}
		}
		else if (isMove && !InputMgr::GetKeyDown(Keyboard::Escape))
		{

			this->PlayerSetIsInven(true);
			inven->SetActive(true);
			inven->ResetRightInven();
			inven->ClearInven();
			SetMove(false);
			rightInvenObj = nullptr;
		}
	}

	//player stamina
	if (isDash)
	{
		stamina -= staminaDownSpeed;
		if (stamina < 0.f)
		{
			stamina = 0.f;
			speed = initSpeed;
		}
	}
	else
	{
		stamina += staminaUpSpeed;
		if (stamina >= maxStamina)
		{
			stamina = maxStamina;
		}
	}
	if (isStun)
	{
		stun -= dt;
		if (stun <= 0)
		{
			isStun = false;
		}
	}
	HideUpdate(dt);
}

void Player::Draw(RenderWindow& window)
{
	HitBoxObject::Draw(window);
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}

	gun->Draw(window);
}

void Player::Dash(float dt)
{
	Translate(direction * speed * dt);
}

void Player::UpdateIdle(float dt)
{
	if (!EqualFloat(direction.x, 0.f))
	{
		SetState(States::Move);
	}
	else if (!EqualFloat(direction.y, 0.f))
	{
		SetState(States::Move);
	}
	if ((lookDir.x > 0 && prevLook.x < 0) ||
		(lookDir.x < 0 && prevLook.x > 0))
	{
		SetState(States::Idle);
	}
}

void Player::UpdateMove(float dt)
{
	if (EqualFloat(direction.x, 0.f) && EqualFloat(direction.y, 0.f))
	{
		SetState(States::Idle);
		return;
	}

	if ((lookDir.x > 0 && prevLook.x < 0) ||
		(lookDir.x < 0 && prevLook.x > 0))
	{
		//animator.Play((lookDir.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		SetState(States::Move);
	}

}

bool Player::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Player::SetHp(int num)
{
	hp -= num;
	if (hp <= 0)
	{
		hp = 0;
	}
}

void Player::SetRad(float rad)
{
	radGuage += rad;
}

void Player::HealHp(int num)
{
	hp += num;
	if (hp > maxHp)
	{
		hp = maxHp;
	}
}

void Player::HealHunger(float num)
{
	hungerGuage += num;
	if (hungerGuage > maxHungerGuage)
	{
		hungerGuage = maxHungerGuage;
	}
}

void Player::HealThirst(float num)
{
	thirstGuage += num;
	if (thirstGuage > maxThirstGuage)
	{
		thirstGuage = maxThirstGuage;
	}
}

void Player::HealEnergy(float num)
{
	energyGuage += num;
	if (energyGuage > maxEnergyGuage)
	{
		energyGuage = maxEnergyGuage;
	}
}

void Player::HealRad(float num)
{
	radGuage += num;
	if (radGuage > maxRadiation)
	{
		radGuage = maxRadiation;
	}
}

void Player::SetPlayerPos()
{
	SetPos(prevPosition);

	//for (auto& hit : hitboxs)
	//{
	//	hit->SetPos(prevPosition);
	//}
	bottom->SetPos(prevPosition);

}

Vector2f Player::SetLookDir()
{
	Vector2f dir;
	dir.x = look.x - GetPos().x;
	dir.y = 0.f;
	return dir;
}

void Player::SetFlipX(bool flip)
{
	SpriteObject::SetFlipX(flip);
}


void Player::SetIsDash(bool dash)
{
	isDash = dash;
}

void Player::SetMove(bool move)
{
	isMove = move;
}

void Player::SetPrevHungerGuage(int hunger)
{
	prevHungerGuage = hunger;
}

void Player::SetPrevThirstGuage(int thirst)
{
	prevThirstGuage = thirst;
}

void Player::SetPrevEnergyGuage(int energy)
{
	prevEnergyGuage = energy;
}

void Player::SetPrevRadGuage(int rad)
{
	prevRadGuage = rad;
}

void Player::Move(float dt)
{
	//Move
	velocity = direction * speed;

	//Stop
	if (Utils::Magnitude(direction) == 0.f)
	{
		velocity = { 0.f, 0.f };
	}
	if (direction.x == 0.f)
	{
		velocity.x = 0.f;
	}
	if (direction.y == 0.f)
	{
		velocity.y = 0.f;
	}
	//////////////////////////////////////////
	//x dir
	prevPosition = GetPos();
	Translate({ velocity.x * dt, 0.f });

	//wall bound
	Collision();

	////////////////////////////////////////
	//y dir
	prevPosition = GetPos();
	Translate({ 0.f, velocity.y * dt, });

	//wall bound
	Collision();
}

void Player::RadDistance()
{
	isRad = false;
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->GetRadPos();

		for (auto& pos : boundInObj)
		{
			float maxRadScale = 0.f;
			auto obj_pos = pos;
			Vector2f obj_center = { pos.x, pos.y - 30.f };

			float dis = Utils::Distance(GetPos(), obj_center);

			if (dis < 150)
			{
				isRad = true;
				maxRadScale = maxRadScale < 2 ? 2 : maxRadScale;
			}
		}
	}
	if (!isRad)
		radiationDelay = 0.f;
}

void Player::Collision()
{
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		FloatRect playerRect = GetGlobalBound();
		playerRect.left -= 60;
		auto boundInObj = ((GameScene*)scene)->ObjListObb(this);

		for (auto& obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
			{
				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "RADIATION"||
					obj->GetName() == "INVISIBLE"//||
					//obj->GetName() == "ENEMY"
					)
					SetPlayerPos();
			}
		}
	}
	//gun fire
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(this);

		for (auto& obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), gun->GetHitbox()))
			{

				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "RADIATION"||
					obj->GetName() == "INVISIBLE")
				{
					//cout << "wall" << endl;
					gun->SetIsInWall(false);
					break;
				}
			}
			else
			{
				//cout << "not wall" << endl;
				gun->SetIsInWall(true);
			}
		}
	}
	if (SCENE_MGR->GetCurrSceneType() == Scenes::Ready)
	{
		auto boundInObj = ((Ready*)scene)->ObjListObb(this);

		for (auto& obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
			{
				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "RADIATION"||
					obj->GetName() == "INVISIBLE")
					SetPlayerPos();
			}
		}
	}

}


void Player::GetItem(map<string, Item>*& items)
{
	inven->SetActive(true);
	auto right_inven = inven->GetRightInven();

	for (auto& item : *items)
	{
		right_inven->AddItem(item.second.type);
	}
}

void Player::UseItems(int num)
{
	if (inven->GetUsedItem(num) == nullptr)
	{
		return;
	}
	if (inven->GetUsedItem(num)->GetCount() == 0)
		return;
	string name = inven->GetUsedItem(num)->GetName();

	//if (name == "Recoverykit")
	//{
	//	HealHp(maxHp / 4);

	//	inven->GetUsedItem(num)->AddCount(-1);
	//	if (inven->GetUsedItem(num)->GetCount() <= 0)
	//		inven->AddDeleteObj(inven->GetUsedItem(num));

	//	return;
	//}
	/////////add other items/////////
	//else if (name == "Apple")
	//{
	//	HealHunger(maxHungerGuage / 4);
	//	inven->GetUsedItem(num)->AddCount(-1);
	//	if (inven->GetUsedItem(num)->GetCount() <= 0)
	//		inven->AddDeleteObj(inven->GetUsedItem(num));
	//	return;
	//}
	//else if (name == "Meat")
	//{
	//	HealHunger(maxHungerGuage / 2);
	//	inven->GetUsedItem(num)->AddCount(-1);
	//	if (inven->GetUsedItem(num)->GetCount() <= 0)
	//		inven->AddDeleteObj(inven->GetUsedItem(num));
	//	return;
	//}
	//else if (name == "Water")
	//{
	//	HealThirst(maxThirstGuage / 3);
	//	inven->GetUsedItem(num)->AddCount(-1);
	//	if (inven->GetUsedItem(num)->GetCount() <= 0)
	//		inven->AddDeleteObj(inven->GetUsedItem(num));
	//	return;
	//}
	//else if (name == "EnergyDrink")
	//{
	//	HealEnergy(maxEnergyGuage / 2);
	//	inven->GetUsedItem(num)->AddCount(-1);
	//	if (inven->GetUsedItem(num)->GetCount() <= 0)
	//		inven->AddDeleteObj(inven->GetUsedItem(num));
	//	return;
	//}
	auto itemEffect = FILE_MGR->GetItemEffect();

	for (auto& k : itemEffect)
	{
		if (name == k.first)
		{
			HealHp(k.second.hp);
			HealHunger(k.second.hungerEffect);
			HealThirst(k.second.thirstEffect);
			HealEnergy(k.second.energyEffect);
			inven->GetUsedItem(num)->AddCount(-1);
			if (inven->GetUsedItem(num)->GetCount() <= 0)
				inven->AddDeleteObj(inven->GetUsedItem(num));
		}
	}

}

void Player::SetStamina(float stamina)
{
	this->stamina = stamina;
}

void Player::SetFireAmmo()
{
	ammo[gun->GetgunType()]--;
}

void Player::SetAmmoType()
{
	ammo[gun->GetgunType()] = maxAmmo[gun->GetgunType()];
}

void Player::Reload()
{
	//int ammoCount = ammo;
	if (ammo[gun->GetgunType()] == maxAmmo[gun->GetgunType()])
	{
		return;
	}

	for (auto bt : *inven->GetPlayerInven()->GetItems())
	{
		if (bt->GetName() == gunBulletName[gun->GetgunType()])
		{
			SOUND_MGR->Play("sounds/reload.wav");

			reloadDelay = reloadDelayTime[gun->GetgunType()] = gun->GetReloadDelay();
			isReloading = true;
			if (bt->GetCount() < maxAmmo[gun->GetgunType()])
			{
				if (ammo[gun->GetgunType()] > 0)
				{
					if (ammo[gun->GetgunType()] + bt->GetCount() >= maxAmmo[gun->GetgunType()])
					{
						ammo[gun->GetgunType()] = maxAmmo[gun->GetgunType()];
						bt->AddCount(-(maxAmmo[gun->GetgunType()] - ammo[gun->GetgunType()]));

					}
					else
					{
						ammo[gun->GetgunType()] = ammo[gun->GetgunType()] + bt->GetCount();
						bt->AddCount(-bt->GetCount());
					}
				}
				else
				{
					ammo[gun->GetgunType()] = bt->GetCount();
					bt->AddCount(-(bt->GetCount()));
				}
			}
			else
			{
				bt->AddCount(-(maxAmmo[gun->GetgunType()] - ammo[gun->GetgunType()]));
				ammo[gun->GetgunType()] = maxAmmo[gun->GetgunType()];
			}
			if (bt->GetCount() <= 0)
			{
				inven->AddDeleteItem(bt);

			}
			return;
		}
	}
}

string Player::GetAmmos()
{
	if (isReloading)
	{
		return "Reloading";
	}
	return (to_string(ammo[gun->GetgunType()]) + "/" + to_string(maxAmmo[gun->GetgunType()]));
}

void Player::Load()
{
	auto playerData = FILE_MGR->GetUserInfo();
	hp = playerData.hp;
	stamina = maxStamina;
	hungerGuage = playerData.hungerGuage;
	thirstGuage = playerData.thirstGuage;
	energyGuage = playerData.energyGuage;
	radGuage = playerData.radGuage;
	clearMaps = playerData.clearMaps;

	lastWephon = playerData.lastWephon;

	money = playerData.money;


	auto invenData = FILE_MGR->GetInvenInfo();

	for (auto& data : invenData)
	{
		string type = data.Type;
		Vector2i invenPos = data.invenPos;
		Vector2i invenGreedPos = data.invenGreedPos;
		int cnt = data.cnt;
		string path = data.path;

		inven->GetPlayerInven()->AddItem(type, cnt, invenPos, invenGreedPos, path);
	}

	auto saveBoxData = FILE_MGR->GetSaveBoxInfo();

	for (auto& data : saveBoxData)
	{
		string type = data.Type;
		Vector2i invenPos = data.invenPos;
		Vector2i invenGreedPos = data.invenGreedPos;
		int cnt = data.cnt;
		string path = data.path;

		inven->GetSaveBox()->AddItem(type, cnt, invenPos, invenGreedPos, path);
	}

	auto useItemData = FILE_MGR->GetUseItemInfo();

	for (auto& data : useItemData)
	{
		if (data.useIdx != -1)
			inven->SetUserItem(data);
	}


	auto allGun = FILE_MGR->GetGunInfoAll();
	auto myGun = inven->GetUsedItem(lastWephon);

	ammo[GunType::Up1_ShotGun] = playerData.sgAmmo_1up;
	ammo[GunType::Shotgun] = playerData.sgAmmo;
	ammo[GunType::Sniper] = playerData.snAmmo;
	ammo[GunType::Rifle] = playerData.rfAmmo;

	maxAmmo[GunType::Up1_ShotGun] = allGun["Up1-Shotgun"].magazine;
	maxAmmo[GunType::Shotgun] = allGun["Shotgun"].magazine;
	maxAmmo[GunType::Sniper] = allGun["Sniper"].magazine;
	maxAmmo[GunType::Rifle] = allGun["Rifle"].magazine;

	gunBulletName[GunType::Up1_ShotGun] = "ShotGunBullet";
	gunBulletName[GunType::Shotgun] = "ShotGunBullet";
	gunBulletName[GunType::Sniper] = "SniperBullet";
	gunBulletName[GunType::Rifle] = "RifleBullet";


	if (myGun == nullptr)
	{
		gun->SetGunType(GunType::None);
	}
	else
	{
		gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
		//SetAmmoType();
	}
}

void Player::Save()
{
	UserInfo nowInfo;
	nowInfo.hp = hp;
	nowInfo.hungerGuage = hungerGuage;
	nowInfo.thirstGuage = thirstGuage;
	nowInfo.energyGuage = energyGuage;
	nowInfo.money = money;
	nowInfo.radGuage = radGuage;
	nowInfo.clearMaps = clearMaps;
	nowInfo.sgAmmo = ammo[GunType::Shotgun];
	nowInfo.snAmmo = ammo[GunType::Sniper];
	nowInfo.rfAmmo = ammo[GunType::Rifle];
	nowInfo.sgAmmo_1up = ammo[GunType::Up1_ShotGun];
	nowInfo.lastWephon = lastWephon;

	FILE_MGR->SaveUserInfo(nowInfo);

	auto nowInven = inven->GetPlayerInven()->GetItems();
	vector<InvenInfo> saveInven;
	for (auto& data : *nowInven)
	{
		if (data->GetCount() <= 0)
			continue;
		InvenInfo d;
		d.Type = data->GetName();
		d.invenPos = data->GetInvenPos();
		d.invenGreedPos = data->GetGreedPos();
		d.cnt = data->GetCount();

		d.path = data->GetPath();

		saveInven.push_back(d);
	}

	FILE_MGR->SaveInvenInfo(saveInven);

	auto nowSaveBox = inven->GetSaveBox()->GetItems();
	vector<InvenInfo> saveBoxInven;
	for (auto& data : *nowSaveBox)
	{
		if (data->GetCount() <= 0)
			continue;
		InvenInfo d;
		d.Type = data->GetName();
		d.invenPos = data->GetInvenPos();
		d.invenGreedPos = data->GetGreedPos();
		d.cnt = data->GetCount();

		d.path = data->GetPath();

		saveBoxInven.push_back(d);
	}

	FILE_MGR->SaveSaveBoxInfo(saveBoxInven);


	auto nowUseItems = inven->GetUseItems();
	vector<InvneUseInfo> saveUseItem;
	int i = 0;
	for (auto& data : nowUseItems)
	{
		if (data == nullptr)
		{
			i++;
			continue;
		}
		if (data->GetCount() <= 0)
			continue;
		InvneUseInfo d;
		d.Type = data->GetName();
		d.invenPos = data->GetInvenPos();
		d.useIdx = i;
		d.cnt = data->GetCount();
		d.path = data->GetPath();
		saveUseItem.push_back(d);
		i++;
	}

	FILE_MGR->SaveUseItemInfo(saveUseItem);
}

void Player::PlayerSetIsInven(bool state)
{
	isInven = state;
}

void Player::AddMoney(int p)
{
	money += p;
	auto txt = inven->GetMoneyTxt();
	txt->SetString(to_string(money));
	txt->SetOrigin(Origins::MR);
}

void Player::SetMoney(int p)
{
	money = p;
	auto txt = inven->GetMoneyTxt();
	txt->SetString(to_string(money));
	txt->SetOrigin(Origins::MR);
}

void Player::Release()
{
	if (gun != nullptr)
		delete gun;
	gun = nullptr;
}

bool Player::GetHide()
{
	cout << isHide << endl;
	return isHide;
}

void Player::SetHide(bool state)
{
	if (isHitBullet)
		return;
	isHide = state;
}

void Player::HideUpdate(float dt)
{
	if (isHitBullet)
	{
		hideDelayTimer += dt;
		if (hideDelayTimer > hideDelay)
		{
			hideDelayTimer = 0.f;
			isHitBullet = false;
		}
	}
}

void Player::HideStop()
{
	isHitBullet = true; hideDelayTimer = 0.f; isHide = false;
}

void Player::SetStun(bool stun, float time)
{
	isStun = stun;
	this->stun = time;
}

void Player::ClearMap(string name)
{
	if (find(clearMaps.begin(), clearMaps.end(), name) == clearMaps.end())
	{
		clearMaps.push_back(name);
	}
}

void Player::SaveSaveBox()
{
	auto nowSaveBox = inven->GetSaveBox()->GetItems();
	vector<InvenInfo> saveBoxInven;
	for (auto& data : *nowSaveBox)
	{
		if (data->GetCount() <= 0)
			continue;
		InvenInfo d;
		d.Type = data->GetName();
		d.invenPos = data->GetInvenPos();
		d.invenGreedPos = data->GetGreedPos();
		d.cnt = data->GetCount();

		d.path = data->GetPath();

		saveBoxInven.push_back(d);
	}

	FILE_MGR->SaveSaveBoxInfo(saveBoxInven);
}