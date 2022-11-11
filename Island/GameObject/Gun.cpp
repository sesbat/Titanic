#include "Gun.h"
#include "Bullet.h"
#include "Player.h"
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
	bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgunbullet.png"));
	//bullet->SetHitbox();
	bullet->Init();
}

Gun::Gun()
	:gunType(GunType::Shotgun),isGunFlip(false), bulletSpeed(10000), range(2000)
{
}

Gun::Gun(GunType type)
	:gunType(GunType::Shotgun),isGunFlip(false), bulletSpeed(10000), range(2000)
{
}

Gun::~Gun()
{
}

void Gun::Init()
{
	bulletPool.OnCreate = OnCreateBullet;
	bulletPool.Init();
	switch (gunType)
	{
	case GunType::Shotgun:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
		Damage = 30;
		shootDelay = 0.5f;
		break;
	case GunType::Rifle:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
		Damage = 50;
		shootDelay = 0.1f;
		break;
	case GunType::Sniper:
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniper.png"));
		shootDelay = 1.f;
		Damage = 500;
		break;
	case GunType::TypeCount:
		break;
	default:
		break;
	}
	SetPos({ player->GetPos().x,player->GetPos().y + 10.f });
	SetOrigin(Origins::MC);
}

void Gun::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(Mouse::Left))
	{
		Fire();
	}
	if (InputMgr::GetMouseButtonDown(Mouse::Right))
	{

		int temp = (int)gunType;
		temp++;
		if (temp == 3)
		{
			temp = 0;
		}
		gunType = (GunType)temp;
	}

	const auto& bulletList = bulletPool.GetUseList();
	for (auto bullet : bulletList)
	{
		bullet->Update(dt);
	}

	SpriteObject::Update(dt);

	//positions
	isGunFlip = player->GetLookDir().x < 0;
	this->SetFlipY(isGunFlip);

	float angle = Utils::Angle(player->GetLookDir());
	GetSprite().setRotation(angle);

	SetPos({ player->GetPos().x,player->GetPos().y + 10.f });
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
}

void Gun::Fire()
{
	//shotgun

	//auto hitObject = &SCENE_MGR->GetCurrScene()->GetObjList()[LayerType::Object][0];

	switch (gunType)
	{
	case GunType::Shotgun:
	{	
		bulletSpeed = 1000;
		range = 200;
		Vector2f startPos = { player->GetPos() };
		startPos += player->GetLookDir() * 50.f;

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

		float temp = atan2(player->GetLookDir().y, player->GetLookDir().x);
		float F1 = temp + M_PI / 12;
		Vector2f randomShot1 = { cos(F1),sin(F1) };
		float F2 = temp - M_PI / 12;
		Vector2f randomShot2 = { cos(F2),sin(F2) };
		float F3 = temp + (M_PI / 12)*2;
		Vector2f randomShot3 = { cos(F3),sin(F3) };
		float F4 = temp - (M_PI / 12) * 2;
		Vector2f randomShot4 = { cos(F4),sin(F4) };


		bullet->Fire(startPos, player->GetLookDir(), bulletSpeed, range);
		bullet1->Fire(startPos, randomShot1, bulletSpeed, range);
		bullet2->Fire(startPos, randomShot2, bulletSpeed, range);
		bullet3->Fire(startPos, randomShot3, bulletSpeed, range);
		bullet4->Fire(startPos, randomShot4, bulletSpeed, range);
	}
	break;
	case GunType::Rifle:
	{
		bulletSpeed = 400;
		range = 700;
		Vector2f startPos = { player->GetPos() };
		startPos += player->GetLookDir() * 50.f;
		Bullet* bullet = bulletPool.Get();
		bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniperbullet.png"));
		bullet->SetOrigin(Origins::MC);
		bullet->Fire(startPos, player->GetLookDir(), bulletSpeed, range);
	}
		break;
	case GunType::Sniper:
	{
		bulletSpeed = 3000;
		range = 1500;
		Vector2f startPos = { player->GetPos() };
		startPos += player->GetLookDir() * 50.f;
		Bullet* bullet = bulletPool.Get();
		bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/sniperbullet.png"));
		bullet->SetOrigin(Origins::MC);
		bullet->Fire(startPos, player->GetLookDir(), bulletSpeed, range);
	}
		break;
	}
}

