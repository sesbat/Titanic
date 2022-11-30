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

void OnCreateBullet(Bullet* bullet)
{
	GameScene* scene = (GameScene*)SCENE_MGR->GetScene(Scenes::GameScene);
	bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
	bullet->Init();
	bullet->SetEnemyList(scene->GetEnemyList());
	bullet->SetPlayer(scene->GetPlayer());
}

Gun::Gun()
	:gunType(GunType::Shotgun),isGunFlip(false), bulletSpeed(10000), range(2000), shootDelay(0)
{
}

Gun::Gun(GunType type, User user)
	:gunType(type), isGunFlip(false), bulletSpeed(10000), range(2000), user(user), shootDelay(0)
{
}

Gun::~Gun()
{
}

void Gun::Init()
{
	bulletPool.OnCreate = OnCreateBullet;
	bulletPool.Init();

	SetGunType(gunType);

	switch (user)
	{
	case User::Player:
		SetPos({ player->GetPos().x,player->GetPos().y + 10.f });
		SetOrigin(Origins::MC);
		break;
	case User::Enemy:
		SetPos({ enemy->GetPos().x,enemy->GetPos().y + 10.f });
		SetOrigin(Origins::MC);
		break;
	}
	
}

void Gun::Update(float dt)
{
	const auto& bulletList = bulletPool.GetUseList();
	for (auto bullet : bulletList)
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
		shootDelay -= dt;
	}
		break;
	}
	
}

void Gun::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
	const auto& bulletList = bulletPool.GetUseList();
	for (auto bullet : bulletList)
	{
		bullet->Draw(window);
	}
}

void Gun::Relaese()
{
	SpriteObject::Release();
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
	if (shootDelay <= 0)
	{
		switch (gunType)
		{
		case GunType::Shotgun:
		{
			Vector2f startPos;
			startPos = transform.transformPoint(100, 0);
			bulletSpeed = 2000;
			range = 800;
			shootDelay = 0.3f;

			Bullet* bullet = bulletPool.Get();
			Bullet* bullet1 = bulletPool.Get();
			Bullet* bullet2 = bulletPool.Get();
			Bullet* bullet3 = bulletPool.Get();
			Bullet* bullet4 = bulletPool.Get();
			bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet1->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet2->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet3->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet4->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));

			float temp = atan2(lookDir.y, lookDir.x);
			float F1 = temp + M_PI / 12;
			Vector2f randomShot1 = { cos(F1),sin(F1) };
			float F2 = temp - M_PI / 12;
			Vector2f randomShot2 = { cos(F2),sin(F2) };
			float F3 = temp + (M_PI / 12) * 2;
			Vector2f randomShot3 = { cos(F3),sin(F3) };
			float F4 = temp - (M_PI / 12) * 2;
			Vector2f randomShot4 = { cos(F4),sin(F4) };

			bullet->SetOrigin(Origins::MR);
			bullet1->SetOrigin(Origins::MR);
			bullet2->SetOrigin(Origins::MR);
			bullet3->SetOrigin(Origins::MR);
			bullet4->SetOrigin(Origins::MR);

			bullet->Fire(startPos, lookDir, bulletSpeed, range, isplayer);
			bullet1->Fire(startPos, randomShot1, bulletSpeed, range, isplayer);
			bullet2->Fire(startPos, randomShot2, bulletSpeed, range, isplayer);
			bullet3->Fire(startPos, randomShot3, bulletSpeed, range, isplayer);
			bullet4->Fire(startPos, randomShot4, bulletSpeed, range, isplayer);
		}
		break;
		case GunType::Rifle:
		{
			Vector2f startPos;
			startPos = transform.transformPoint(100, 0);
			bulletSpeed = 2000;
			range = 1000;
			shootDelay = 0.1f;

			Bullet* bullet = bulletPool.Get();
			bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
			bullet->SetOrigin(Origins::MR);
			bullet->Fire(startPos, lookDir, bulletSpeed, range, isplayer);
		}
		break;
		case GunType::Sniper:
		{
			Vector2f startPos;
			startPos = transform.transformPoint(120, 0);
			bulletSpeed = 5000;
			range = 2000;
			shootDelay = 1.f;

			Bullet* bullet = bulletPool.Get();
			bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniperbullet.png"));
			bullet->SetOrigin(Origins::MR);
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
	case GunType::Shotgun:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
		Damage = 30;
		//shootDelay = 0.8f;
		break;
	case GunType::Rifle:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/rifel.png"));
		Damage = 50;
		//shootDelay = 0.3f;
		break;
	case GunType::Sniper:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniper.png"));
		//shootDelay = 1.f;
		Damage = 500;
		break;
	case GunType::TypeCount:
		break;
	default:
		break;
	}
}

