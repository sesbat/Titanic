 #include "Enemy.h"
#include "Player.h"
#include "HitBox.h"
#include "Object.h"
#include "Gun.h"
#include "VertexArrayObj.h"
#include "../Scens/SceneManager.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundManager.h"
#include <iostream>

Enemy::Enemy()
	: currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), moveTime(0.f), hitTime(0.f), getAttackTime(1.f), attack(true), hp(15), 
	maxHp(15), barScaleX(60.f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Player* player)
{
	HitBoxObject::Init();
	this->player = player;

	hp = maxHp;

	gun = new Gun(GunType::Rifle, User::Enemy);
	gun->SetEnemy(this);
	gun->Init();

	animator.SetTarget(&sprite);

	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	//animation
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("EnemyIdle"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("EnemyIdleLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("EnemyMove"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("EnemyMoveLeft"));

	scene = SCENE_MGR->GetCurrScene();
	SetState(States::Idle);
}

void Enemy::SetState(States newState)
{
	currState = newState;

	switch ( currState )
	{
	case Enemy::States::Idle:
		animator.PlayQueue((direction.x > 0.f) ? "EnemyIdle" : "EnemyIdleLeft");
		break;
	case Enemy::States::Move:
		animator.Play((direction.x > 0.f) ? "EnemyMove" : "EnemyMoveLeft");
		break;
	case Enemy::States::Dead:
		SetActive(false);
		break;
	}
}

Enemy::States Enemy::GetState()
{
    return currState;
}

void Enemy::Update(float dt)
{
	if (!enabled || !IsInView())
		return;

	HitBoxObject::Update(dt);

	direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
	//enemy dead
	if (hp <= 0)
	{
		SetState(States::Dead);
	}

	//enemy attack
	if ( currState != States::Dead )
	{
		AttackPattern(dt);
	}

	//move
	if (currState == States::Move)
	{
		Move(dt);
	}

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);
	
	//gun
	gun->Update(dt);
}

void Enemy::Draw(RenderWindow& window)
{

	if (!enabled || !IsInView())
		return;
	if ( GetActive() )
	{
		HitBoxObject::Draw(window);
		window.draw(healthBar);
	}
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}
	gun->Draw(window);
}

void Enemy::OnCompleteDead()
{
	SetActive(false);
}

bool Enemy::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Enemy::SetHp(int num)
{
	if ( hp > 0 )
	{
		hp -= num;
	}
	else
	{
		hp = 0;
	}
}

void Enemy::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	healthBar.setSize({ (barScaleX / maxHp) * hp, 15.f });
	if ( hp > (maxHp / 2) )
	{
		healthBar.setFillColor(Color::Green);
	}
	else if ( hp <= (maxHp / 2) && hp > (maxHp / 5) )
	{
		healthBar.setFillColor(Color::Yellow);
	}
	else
	{
		healthBar.setFillColor(Color::Red);
	}
	if ( hp <= 0 )
	{
		healthBar.setOutlineThickness(0.f);
	}
	else
	{
		healthBar.setOutlineThickness(2.f);
	}
}

void Enemy::SetEnemyPos()
{
	SetPos(prevPosition);

	for (auto& hit : hitboxs)
	{
		hit->SetPos(prevPosition);
	}
	bottom->SetPos(prevPosition);

	healthBar.setPosition({ prevPosition.x, prevPosition.y - 35.f });
}

void Enemy::AttackPattern(float dt)
{
	//attack motion
	if ( Utils::Distance(player->GetPos(), GetPos()) < 300.f )
	{
		SetState(States::Move);
		//moveTime = 3.f;
	}
	else
	{
		SetState(States::Idle);
	}

	//boss hits player
	hitTime += dt;
	
}

void Enemy::Move(float dt)
{
	dir = Utils::Normalize(player->GetPos() - GetPos());

	//x dir
	prevPosition = GetPos();
	Translate({ dir.x * this->speed * dt, 0.f });
	//position
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
	//wall bound
	Collision();

	//y dir
	prevPosition = GetPos();
	Translate({ 0.f,  dir.y * this->speed * dt });
	//position
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
	Collision();
}

void Enemy::Collision()
{
	auto obj = scene->GetObjList();
	//wall bound
	for (auto& objects : obj[LayerType::Object][0])
	{
		auto hit = ((HitBoxObject*)objects)->GetBottom();
		if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
			continue;
		if (objects->GetName() == "TREE" ||
			objects->GetName() == "STONE" ||
			objects->GetName() == "BLOCK" ||
			objects->GetName() == "PLAYER")
		{
			if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
			{
				SetEnemyPos();
				break;
			}
		}
	}
}
