#include "MonsterHouse.h"
#include "Player.h"
#include "../Scens/SceneManager.h"
#include "../Scens/GameScene.h"
#include "../GameObject/HitBox.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/SoundManager.h"	
#include <iostream>


MonsterHouse::MonsterHouse()
	:spawnTimer(5.f), nowTime(0.f)
{
	isHide = false;
}

MonsterHouse::~MonsterHouse()
{
}

void MonsterHouse::Update(float dt)
{
	if (isdead)
		return;

	if (!enabled || !IsInView())
		return;
	HitBoxObject::Update(dt);
	if (!player->GetHide() && isInSight && (Utils::Distance(player->GetPos(), GetPos()) < searchDis))
	{
		isfindPlayer = true;
	}
	else
	{
		isfindPlayer = false;
	}

	if (isfindPlayer)
	{
		nowTime += dt;
		if (spawnTimer < nowTime)
		{
			nowTime = 0.f;

			{
				Enemy* enemy = new Enemy();
				enemy->SetName("ENEMY");
				enemy->SetId(((GameScene*)SCENE_MGR->GetCurrScene())->GetID()++);
				enemy->SetPos(GetPos());
				enemy->SetHitBox("graphics/enemy1.png");
				enemy->SetType((GunType)1);
				enemy->SetEnemyType("graphics/enemy1.png");
				enemy->SetGreedObject(isGreedObject);
				enemy->SetItem(map<string, Item>());
				enemy->Init(player);
				(SCENE_MGR->GetCurrScene())->AddGameObject(enemy, LayerType::Object, 0);
				((GameScene*)SCENE_MGR->GetCurrScene())->GetEnemyList()->push_back(enemy);
				((GameScene*)SCENE_MGR->GetCurrScene())->GetTree().insert(enemy);
			}
		}
	}
	else
	{
		
	}
}

void MonsterHouse::Draw(RenderWindow& window)
{
	if (!enabled || !IsInView())
		return;
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}
	VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		HitBoxObject::Draw(window);
		if (!isdead)
			window.draw(healthBar);
		lines[0].position = { GetPos() };
		lines[1].position = { player->GetPos() };
		window.draw(lines);
	}
}

void MonsterHouse::Release()
{
}

void MonsterHouse::Init(Player* player)
{
	HitBoxObject::Init();
	this->player = player;
	auto enemys = FILE_MGR->GetEnemyInfo();
	auto data = enemys[enemyType];

	hp = maxHp = data.maxHp;
	spawnTimer = data.hitTime;

	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 75.f });

	Utils::SetOrigin(healthBar, Origins::BC);
	scene = SCENE_MGR->GetCurrScene();

	bottomPos = bottom->GetHitBottomPos();

	SetTexture(*RESOURCES_MGR->GetTexture(enemyType));
	SetOrigin(Origins::BC);
}

void MonsterHouse::SetHp(int num)
{
	if (isdead)
		return;

	hp -= num;
	SetHpBar();
	healthBar.setPosition({ GetPos().x, GetPos().y - 75.f });
	if (hp <= 0)
	{
		hp = 0;
		SOUND_MGR->Play("sounds/death.wav");
		SetTexture(*RESOURCES_MGR->GetTexture("graphics/enemy/s_infestation_dead.png"));
		isdead = true;
	}
}