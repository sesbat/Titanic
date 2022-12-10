#include "Gun.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/SoundManager.h"
#include "../Scens/SceneManager.h"
#include "VertexArrayObj.h"
#include "Object.h"
#include "HitBox.h"
#include "../Scens/GameScene.h"
#include <iostream>
#include "../Framework/ObjectPool.h"
#include "HitBoxObject.h"
#include "../Scens/Ready.h"

void OnCreateBullet(Bullet* bullet)
{
	GameScene* scene = (GameScene*)SCENE_MGR->GetScene(Scenes::GameScene);
	bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
	bullet->Init();
	bullet->SetEnemyList(scene->GetEnemyList());
	bullet->SetPlayer(scene->GetPlayer());
}

Gun::Gun()
	:gunType(GunType::Shotgun), isGunFlip(false), bulletSpeed(10000), range(2000), shootDelay(0), isInWall(true)
{
}

Gun::Gun(GunType type, User user)
	:gunType(type), isGunFlip(false), bulletSpeed(10000), range(2000), user(user), shootDelay(0), isInWall(true)
{
}

Gun::~Gun()
{
	Relaese();
}

void Gun::Init()
{
	gunsInfo = FILE_MGR->GetGunInfoAll();

	bulletPool.OnCreate = OnCreateBullet;
	bulletPool.Init();

	SetGunType(gunType);
	
	gunhitbox = new HitBox();
	gunhitbox->SetActive(true);
	gunhitbox->SetHitbox({ 0.f,0.f,10.f,10.f });

	switch (user)
	{
	case User::Player:
		SetPos({ player->GetPos().x,player->GetPos().y + 10.f });
		SetOrigin(Origins::MC);
		gunhitbox->SetPos(GetPos());
		break;
	case User::Enemy:
		SetPos({ enemy->GetPos().x,enemy->GetPos().y + 10.f });
		SetOrigin(Origins::MC);
		gunhitbox->SetPos(GetPos());
		break;
	}

	scene = SCENE_MGR->GetCurrScene();

	gunNameType["GUN1"] = GunType::Sniper;
	gunNameType["GUN2"] = GunType::Shotgun;
	gunNameType["GUN3"] = GunType::Rifle;
	gunNameType["GUN4"] = GunType::Sniper;
	
}

void Gun::Update(float dt)
{
	const auto& bulletList = bulletPool.GetUseList();
	for (auto& bullet : bulletList)
	{
		bullet->Update(dt);
	}

	SpriteObject::Update(dt);
	
	switch (user)
	{
	case User::Player:
	{
		//positions
		isGunFlip = player->GetLookDir().x < 0;
		lookDir = player->GetLookDir();
		this->SetFlipY(isGunFlip);

		float angle = Utils::Angle(player->GetLookDir());
		GetSprite().setRotation(angle);

		SetPos({ player->GetPos().x,player->GetPos().y + 10.f });
		
		Transform translation;
		translation.translate(GetPos());
		float angle2 = Utils::Angle(lookDir);
		Transform rotation;
		rotation.rotate(angle2);

		Transform transform = translation * rotation;
		gunhitbox->SetPos(transform.transformPoint(55.f, 5.f));
		
		shootDelay -= dt;
	}
		break;
	case User::Enemy:
	{
		isGunFlip = enemy->GetLookDir().x < 0;
		lookDir = enemy->GetLookDir();
		this->SetFlipY(isGunFlip);

		float angle = Utils::Angle(enemy->GetLookDir());
		GetSprite().setRotation(angle);

		SetPos({ enemy->GetPos().x,enemy->GetPos().y + 10.f });

		Transform translation;
		translation.translate(GetPos());
		float angle2 = Utils::Angle(lookDir);
		Transform rotation;
		rotation.rotate(angle2);

		Transform transform = translation * rotation;
		gunhitbox->SetPos(transform.transformPoint(55.f, 5.f));

		shootDelay -= dt;
	}
		break;
	}
}

void Gun::Draw(RenderWindow& window)
{
	 SpriteObject::Draw(window);
	const auto& bulletList = bulletPool.GetUseList();
	for (auto& bullet : bulletList)
	{
		if (bullet->GetActive())
		{
			bullet->Draw(window);
		}
	}
}

void Gun::Relaese()
{
	delete gunhitbox;
	bulletPool.Release();
}

void Gun::SetPlayer(Player* player)
{
	this->player = player;
	this->enemy = nullptr;
}

void Gun::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;
	this->player = nullptr;
	
}

void Gun::Fire(Vector2f pos, bool isplayer)
{
	Transform translation;
	translation.translate(pos);

	float angle = Utils::Angle(lookDir);
	Transform rotation;
	rotation.rotate(angle);

	Transform transform = translation * rotation;

	if (shootDelay <= 0 && isInWall)
	{
		switch (gunType)
		{
		case GunType::None:
			break;

		case GunType::Shotgun:
		{
			auto data = gunsInfo["GUN1"];
			Vector2f startPos;
			startPos = transform.transformPoint(80, 0);
			bulletSpeed = data.speed;
			range = data.range;
			shootDelay = data.shootDelay;

			int cnt = data.cnt;
			float dir = data.shotgunDir;
			int harfCnt = cnt / 2;

			for (int i = 1; i <= cnt; i++)
			{
				Bullet* bullet = bulletPool.Get();
				float temp = atan2(lookDir.y, lookDir.x);
				Vector2f randomShot;
				float F1 = temp + M_PI / 12 * i * (i <= harfCnt ? -1 : 1);
				randomShot = { cos(F1),sin(F1) };

				bullet->SetOrigin(Origins::MC);
				bullet->SetDamage(damage);
				bullet->Fire(startPos, lookDir, bulletSpeed, range, isplayer);
			}
			SOUND_MGR->Play(data.soundPath);

		}
		break;
		case GunType::Rifle:
		{
			auto data = gunsInfo["GUN2"];
			Vector2f startPos;
			startPos = transform.transformPoint(80, 0);
			bulletSpeed = data.speed;
			range = data.range;
			shootDelay = data.shootDelay;

			Bullet* bullet = bulletPool.Get();
			//bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet->SetOrigin(Origins::MR);

			bullet->SetDamage(damage);

			SOUND_MGR->Play(data.soundPath);

			bullet->Fire(startPos, lookDir, bulletSpeed, range, isplayer);
		}
		break;
		case GunType::Sniper:
		{
			auto data = gunsInfo["GUN3"];
			Vector2f startPos;
			startPos = transform.transformPoint(80, 0);
			bulletSpeed = data.speed;
			range = data.range;
			shootDelay = data.shootDelay;

			Bullet* bullet = bulletPool.Get();
			//bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniperbullet.png"));
			bullet->SetOrigin(Origins::MR);
			bullet->SetDamage(damage);
			
			SOUND_MGR->Play(data.soundPath);
			bullet->Fire(startPos, lookDir, bulletSpeed, range, isplayer);
		}
		break;
		}
	}
	
}

void Gun::SetGunType(GunType type)
{
	gunType = type;
	switch (gunType)
	{
	case GunType::None:
		SetOrigin(Origins::MC);
		SetActive(false);
		break;
	case GunType::Shotgun:
		SetActive(true);
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
		damage = gunsInfo["GUN1"].damage;
		SetOrigin(Origins::MC);
		break;
	case GunType::Rifle:
		SetActive(true);
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/rifel.png"));
		damage = gunsInfo["GUN2"].damage;
		SetOrigin(Origins::MC);
		break;
	case GunType::Sniper:
		SetActive(true);
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniper.png"));
		damage = gunsInfo["GUN3"].damage;
		SetOrigin(Origins::MC);
		break;
	case GunType::TypeCount:
		break;
	default:
		break;
	}
}


GunType Gun::ItemNameToType(string name)
{
	return gunNameType[name];
}

const RectangleShape& Gun::GetHitbox() const
{
	return gunhitbox->GetHitbox();
}
