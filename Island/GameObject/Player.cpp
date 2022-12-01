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
#include <iostream>
#include "Gun.h"
#include "../Ui/Inventory.h"
#include "../Ui/InventoryBox.h"
#include "../Ui/InvenItem.h"
#include "../GameObject/ItemBoxObject.h"


Player::Player()
	: currState(States::None), speed(200.f), maxSpeed(200.f),
	look(1.f, 0.f), prevLook(1.f, 0.f),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	hp(10), maxHp(10), isDash(false), stamina(10.f), maxStamina(10.f),
	hungerGuage(255), thirstGuage(255), energyGuage(255),
	staminaScale(1.f), staminaTime(5.f), dash(0.01f),
	hungerDelay(30.f), ThirstDelay(20.f), EnergyDelay(40.f), isAlive(true), isMove(true),
	maxSGAmmo(10), maxRFAmmo(45), maxSNAmmo(5)
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
	ammo = maxSGAmmo;
	//rfAmmo = maxRFAmmo;
	//snAmmo = maxSNAmmo;
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

	/*if (InputMgr::GetKeyDown(Keyboard::Num0))
	{
		hungerGuage = 255;
	}
	if (InputMgr::GetKeyDown(Keyboard::Num9))
	{
		hungerGuage = 50;
	}*/

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
	if(isMove)
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

	//input
	switch (gun->GetgunType())
	{
	case GunType::Shotgun:
	case GunType::Sniper:
		if (InputMgr::GetMouseButtonDown(Mouse::Left) && ammo > 0 && !inven->GetActive())
		{
			gun->Fire(GetPos(), true);
			ammo--;
		}
		break;
	case GunType::Rifle:
		if (InputMgr::GetMouseButton(Mouse::Left) && ammo > 0 && !inven->GetActive())
		{
			gun->Fire(GetPos(), true);
			ammo--;
		}
		break;
	}
	if (InputMgr::GetKeyDown(Keyboard::Num1))
	{
		auto myGun = inven->GetUsedItem(0);

		if (myGun == nullptr)
		{
			gun->SetGunType(GunType::None);
		}
		else
		{
			gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
		}
	}
	if (InputMgr::GetKeyDown(Keyboard::Num2))
	{
		auto myGun = inven->GetUsedItem(1);

		if (myGun == nullptr)
		{
			gun->SetGunType(GunType::None);
		}
		else
		{
			gun->SetGunType(gun->ItemNameToType(myGun->GetName()));
		}
	}
	//use item
	if (InputMgr::GetKeyDown(Keyboard::Num3))
	{
		UseItems(4);
		//gun->SetGunType(GunType::Sniper);
	}
	if (InputMgr::GetKeyDown(Keyboard::Num4))
	{
		UseItems(5);
		//gun->SetGunType(GunType::Shotgun);
	}
	if (InputMgr::GetKeyDown(Keyboard::Num5))
	{
		UseItems(6);
		//gun->SetGunType(GunType::Rifle);
	}
	if (InputMgr::GetKeyDown(Keyboard::Num6))
	{
		UseItems(7);
		//gun->SetGunType(GunType::Sniper);
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
		inven->SetActive(!(inven->GetActive()));
		inven->ResetRightInven();
		if (!inven->GetActive())
		{
			if (inven->GetRightInven()->GetItems()->size() > 0)
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
			if(rightInvenObj != nullptr)
			{
				auto items = rightInvenObj->GetInvenBox()->GetItems();
				if (items->size() == 0)
				{
					((GameScene*)scene)->EmpytyInven(rightInvenObj);
					rightInvenObj = nullptr;
				}
			}
			inven->ClearInven();
		}
		else
		{
			rightInvenObj = nullptr;
		}
	}
	
	//player stamina
	if (isDash)
	{
		stamina -= 0.01f;
		if (stamina < 0.f)
		{
			stamina = 0.f;
			speed = 500.f;
		}
	}
	else
	{
		stamina += 0.01f;
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
	//window.draw(light);
}

void Player::Dash(float dt)
{
	Translate(direction * speed * dt);
}

void Player::UpdateIdle(float dt)
{
	//attackHitbox->SetActive(false);
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
	if (hp > 0)
	{
		hp -= num;
	}
	else
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
	if(inven->GetUsedItem(num)->GetCount() == 0)
		return;
	string name = inven->GetUsedItem(num)->GetName();
	if (name == "Recoverykit")
	{
		HealHp(2);
		inven->GetUsedItem(num)->AddCount(-1);
		if (inven->GetUsedItem(num)->GetCount() <= 0)
			inven->AddDeleteObj(inven->GetUsedItem(num));

		return;
	}
	///////add other items/////////
	else if (name == "Recoverykit")
	{
		HealHp(2);
		return;
	}
	else if (name == "Recoverykit")
	{
		HealHp(2);
		return;
	}
	else if (name == "Recoverykit")
	{
		HealHp(2);
		return;
	}
	
}

void Player::SetAmmo()
{
	switch (gun->GetgunType())
	{
	case GunType::Shotgun:
		
		break;
	case GunType::Rifle:
		
		break;
	case GunType::Sniper:
		
		break;
	}
}

void Player::Reload()
{
	switch (gun->GetgunType())
	{
	case GunType::Shotgun:
		break;
	case GunType::Rifle:
		break;
	case GunType::Sniper:
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
		if(data.useIdx != -1)
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
