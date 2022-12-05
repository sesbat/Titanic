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
	: currState(States::None), speed(400.f), maxSpeed(200.f),
	look(1.f, 0.f), prevLook(1.f, 0.f),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	hp(500), maxHp(500), isDash(false), stamina(10.f), maxStamina(10.f),
	hungerGuage(255), thirstGuage(255), energyGuage(255),
	maxHungerGuage(255), maxThirstGuage(255), maxEnergyGuage(255),
	staminaScale(1.f), staminaTime(5.f), dash(0.01f),
	hungerDelay(30.f), ThirstDelay(20.f), EnergyDelay(40.f), isAlive(true), isMove(true), ammo(0),
	magazineSG(10), magazineRF(45), magazineSN(5), shootDelay(0.f), reloadDelay(0.f), isReloading(false)
{
}

Player::~Player()
{
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
	direction.x = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.y = InputMgr::GetAxisRaw(Axis::Vertical);

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	}

	hungerDelay -= dt;
	ThirstDelay -= dt;
	EnergyDelay -= dt;
	if (hungerDelay < 0.f && hungerGuage > 0.f)
	{
		hungerGuage -= 1.f;
		hungerDelay = 30.f;
	}
	if (ThirstDelay < 0.f && thirstGuage > 0.f)
	{
		thirstGuage -= 1.f;
		ThirstDelay = 20.f;
	}
	if (EnergyDelay < 0.f && energyGuage > 0.f)
	{
		energyGuage -= 1.f;
		EnergyDelay = 40.f;
	}

	if (staminaScale < 0.99f && !isDash)
	{
		staminaScale += dash;
	}
	if (staminaScale > 0.f && isDash)
	{
		staminaScale -= dash / staminaTime;
		if (staminaScale < 0.f)
			isDash = !isDash;
	}

	//Dead
	if (hp <= 0 || hungerGuage <= 0 ||
		thirstGuage <= 0 || energyGuage <= 0)
	{
		isAlive = false;
	}

	//Move
	if (isMove)
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
	SetAmmoType();
	//input
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		shootDelay -= dt;
		reloadDelay -= dt;
		//fire
		if (ammo > 0 && shootDelay <= 0 && reloadDelay <= 0 && gun->GetIsInWall())
		{
			isReloading = false;
			switch (gun->GetgunType())
			{
			case GunType::Shotgun:
			case GunType::Sniper:
				if (InputMgr::GetMouseButtonDown(Mouse::Left) && !inven->GetActive())
				{
					SetFireAmmo();
					gun->Fire(GetPos(), true);
					shootDelay = gun->GetpShootDelay();
				}
				break;
			case GunType::Rifle:
				if (InputMgr::GetMouseButton(Mouse::Left) && !inven->GetActive())
				{
					SetFireAmmo();
					gun->Fire(GetPos(), true);
					shootDelay = gun->GetpShootDelay();

				}
				break;
			}
		}
		//reload
		if (InputMgr::GetKeyDown(Keyboard::R))
		{
			Reload();
		}
	}
	
	if (!isReloading)
	{
		if (InputMgr::GetKeyDown(Keyboard::Num1))
		{
			auto myGun = inven->GetUsedItem(0);

			if (myGun == nullptr)
			{
				gun->SetGunType(GunType::None);
				ammo = 0;
			}
			else
			{
				gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
				SetAmmoType();
			}
		}
		if (InputMgr::GetKeyDown(Keyboard::Num2))
		{
			auto myGun = inven->GetUsedItem(1);

			if (myGun == nullptr)
			{
				gun->SetGunType(GunType::None);
				ammo = 0;
			}
			else
			{
				gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
				SetAmmoType();
			}
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
		speed = 400.f;
		isDash = true;
	}
	if (InputMgr::GetKeyUp(Keyboard::LShift))
	{
		speed = maxSpeed;
		isDash = false;
	}

	if (InputMgr::GetKeyDown(Keyboard::Tab))
	{
		if (inven->GetActive() && !isMove)	
		{
			SetMove(true);
			inven->SetActive(!(inven->GetActive()));
			//inven->ResetRightInven();
			//cout << inven->GetRightInven()->GetName() << endl;
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
		else if (isMove)
		{
			SetIsInven(true);
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
		stamina -= 0.1f;
		if (stamina < 0.f)
		{
			stamina = 0.f;
			speed = maxSpeed;
		}
	}
	else
	{
		stamina += 0.05f;
		if (stamina >= maxStamina)
		{
			stamina = maxStamina;
		}
	}

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

	//for (auto& hit : hitboxs)
	//{
	//	hit->SetPos(GetPos());
	//}
	//wall bound
	Collision();

	////////////////////////////////////////
	//y dir
	prevPosition = GetPos();
	Translate({ 0.f, velocity.y * dt, });

	//for (auto& hit : hitboxs)
	//{
	//	hit->SetPos(GetPos());
	//}
	//wall bound
	Collision();
}

void Player::Collision()
{
	//wall bound
	//auto obj = scene->GetObjList();

	//for (auto& objects : obj[LayerType::Object][0])
	//{
	//	auto hit = ((HitBoxObject*)objects)->GetBottom();
	//	if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
	//		continue;
	//	if (!objects->GetActive())
	//	{
	//		continue;
	//	}
	//	if (objects->GetName() == "TREE" ||
	//		objects->GetName() == "STONE" ||
	//		objects->GetName() == "BLOCK" ||
	//		objects->GetName() == "ENEMY")
	//	{
	//		if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
	//		{
	//			//cout << objects->GetName() << endl;
	//			SetPlayerPos();
	//			break;
	//		}
	//	}
	//}

	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(this);

		for (auto obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
			{
				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" //||
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

		for (auto obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), gun->GetHitbox()))
			{

				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK")
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

		for (auto obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
			{
				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "ENEMY")
					SetPlayerPos();
			}
		}
	}
}


void Player::GetItem(map<string, Item>* items)
{
	inven->SetActive(true);
	auto right_inven = inven->GetRightInven();

	for (auto item : *items)
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
	if (name == "Recoverykit")
	{
		HealHp(maxHp / 4);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));

		return;
	}
	///////add other items/////////
	else if (name == "Apple")
	{
		HealHunger(maxHungerGuage / 4);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));
		return;
	}
	else if (name == "Meat")
	{
		HealHunger(maxHungerGuage / 2);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));
		return;
	}
	else if (name == "Water")
	{
		HealThirst(maxThirstGuage / 3);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));
		return;
	}
	else if (name == "EnergyDrink")
	{
		HealEnergy(maxEnergyGuage / 2);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));
		return;
	}
}

void Player::SetFireAmmo()
{
	switch (gun->GetgunType())
	{
	case GunType::Shotgun:
		ammo--;
		sgAmmo = ammo;
		break;
	case GunType::Rifle:
		ammo--;
		rfAmmo = ammo;
		break;
	case GunType::Sniper:
		ammo--;
		snAmmo = ammo;
		break;
	}
}

void Player::SetAmmoType()
{
	switch (gun->GetgunType())
	{
	case GunType::Shotgun:
		ammo = sgAmmo;
		break;
	case GunType::Rifle:
		ammo = rfAmmo;
		break;
	case GunType::Sniper:
		ammo = snAmmo;
		break;
	}
}

void Player::Reload()
{
	int ammoCount = ammo;
	switch (gun->GetgunType())
	{
	case GunType::None:
		break;
	case GunType::Shotgun:
		if (ammo == magazineSG)
		{
			return;
		}
		for (auto bt : *inven->GetPlayerInven()->GetItems())
		{
			if (bt->GetName() == "ShotGunBullet")
			{
				reloadDelay = 1.5f;
				isReloading = true;
				if (bt->GetCount() < magazineSG)
				{
					if (ammo > 0)
					{
						if (ammo + bt->GetCount() >= magazineSG)
						{
							ammo = sgAmmo = magazineSG;
							bt->AddCount(-(magazineSG - ammoCount));
							
						}
						else
						{
							ammo = sgAmmo = ammo + bt->GetCount();
							bt->AddCount(-(magazineSG));
						}
					}
					else
					{
						ammo = bt->GetCount();
						sgAmmo = ammo;
						bt->AddCount(-(magazineSG));
					}
				}
				else
				{
					ammo = sgAmmo = magazineSG;
					bt->AddCount(-(magazineSG - ammoCount));
				}
				if (bt->GetCount() <= 0)
				{
					inven->AddDeleteItem(bt);

				}
				return;
			}
		}
		break;
	case GunType::Rifle:
		if (ammo == magazineRF)
		{
			return;
		}
		for (auto bt : *inven->GetPlayerInven()->GetItems())
		{
			if (bt->GetName() == "RifleBullet")
			{
				reloadDelay = 1.f;
				isReloading = true;
				if (bt->GetCount() < magazineRF)
				{
					if (ammo > 0)
					{
						if (ammo + bt->GetCount() >= magazineRF)
						{
							ammo = rfAmmo = magazineRF;
							bt->AddCount(-(magazineRF - ammoCount));
						}
						else
						{
							ammo = rfAmmo = ammo + bt->GetCount();
							bt->AddCount(-(magazineRF));
						}
					}
					else
					{
						ammo = bt->GetCount();
						rfAmmo = ammo;
						bt->AddCount(-(magazineRF));
					}
				}
				else
				{
					ammo = rfAmmo = magazineRF;
					bt->AddCount(-(magazineRF - ammoCount));
				}
				if (bt->GetCount() <= 0)
				{
					inven->AddDeleteItem(bt);

				}
				return;
			}
		}
		break;
	case GunType::Sniper:
		if (ammo == magazineSN)
		{
			return;
		}
		for (auto bt : *inven->GetPlayerInven()->GetItems())
		{
			if (bt->GetName() == "SniperBullet")
			{
				reloadDelay = 3.f;
				isReloading = true;
				if (bt->GetCount() < magazineSN)
				{
					if (ammo > 0)
					{
						if (ammo + bt->GetCount() >= magazineSN)
						{
							ammo = snAmmo = magazineSN;
							bt->AddCount(-(magazineSN - ammoCount));
							
						}
						else
						{
							ammo = snAmmo = ammo + bt->GetCount();
							bt->AddCount(-(magazineSN));
						}
					}
					else
					{
						ammo = bt->GetCount();
						snAmmo = ammo;
						bt->AddCount(-(magazineSN));
					}
				}
				else
				{
					ammo = snAmmo = magazineSN;
					bt->AddCount(-(magazineSN - ammoCount));
				}
				if (bt->GetCount() <= 0)
				{
					inven->AddDeleteItem(bt);

				}
				return;
			}
		}
		break;
	}
}

string Player::GetAmmos()
{
	if (isReloading)
	{
		return "Reloading";
	}
	switch (gun->GetgunType())
	{
	case GunType::None:
		return ("No Guns");
		break;
	case GunType::Shotgun:
		return (to_string(ammo) + "/" + to_string(magazineSG));
		break;
	case GunType::Rifle:
		return (to_string(ammo) + "/" + to_string(magazineRF));
		break;
	case GunType::Sniper:
		return (to_string(ammo) + "/" + to_string(magazineSN));
		break;
	}
}

void Player::Load()
{
	auto playerData = FILE_MGR->GetUserInfo();
	hp = playerData.hp;
	stamina = maxStamina;
	hungerGuage = playerData.hungerGuage;
	thirstGuage = playerData.thirstGuage;
	energyGuage = playerData.energyGuage;
	money = playerData.money;

	auto invenData = FILE_MGR->GetInvenInfo();

	for (auto data : invenData)
	{
		string type = data.Type;
		Vector2i invenPos = data.invenPos;
		Vector2i invenGreedPos = data.invenGreedPos;
		int cnt = data.cnt;
		string path = data.path;

		inven->GetPlayerInven()->AddItem(type, cnt, invenPos, invenGreedPos, path);
	}


	auto useItemData = FILE_MGR->GetUseItemInfo();

	for (auto data : useItemData)
	{
		if (data.useIdx != -1)
			inven->SetUserItem(data);
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

	FILE_MGR->SaveUserInfo(nowInfo);

	auto nowInven = inven->GetPlayerInven()->GetItems();
	vector<InvenInfo> saveInven;
	for (auto data : *nowInven)
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

	auto nowUseItems = inven->GetUseItems();
	vector<InvneUseInfo> saveUseItem;
	int i = 0;
	for (auto data : nowUseItems)
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
