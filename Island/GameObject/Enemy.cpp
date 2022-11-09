#include "Enemy.h"
#include "Player.h"
#include "VertexArrayObj.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/SoundManager.h"
#include <iostream>

Enemy::Enemy()
    : currState(States::None), speed(50.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), bossState(0), moveTime(0.f), hitTime(0.f), getAttackTime(1.f), attack(true), hp(15), maxHp(15), barScaleX(60.f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Player* player)
{
	SetPos({ 200.f,200.f });
	this->player = player;

	hp = maxHp;

	//sprite.setScale({ 2.f,2.f });

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
	AddHitBox("graphics/enemy/enemy1.png");

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
	}
}

Enemy::States Enemy::GetState()
{
    return currState;
}

void Enemy::SetBackground(SpriteObject* bk)
{
	background = bk;
}

void Enemy::Update(float dt)
{
	prevPosition = GetPos();

	SpriteObject::Update(dt);
	direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;

	//boss attack
	if ( currState != States::Dead )
	{
		AttackPattern(dt);
	}

	//player attack hitbox hits boss
	getAttackTime += dt;
	/*if ( getAttackTime > 0.5f )
	{
		if ( bossHitbox->GetActive() && player->GetAttackHitbox()->GetActive() )
		{
			if ( Utils::OBB(bossHitbox->GetHitbox(), player->GetAttackHitbox()->GetHitbox()) )
			{
				cout << "player hit boss" << endl;
				SetHp(player->GetDamage());
				if ( hp <= 0 )
				{
					SetState(States::Dead);
					bossHitbox->SetActive(false);
					attackHitbox->SetActive(false);
					ITEM_GEN->Generate(GetPos(), true);
				}
				getAttackTime = 0.f;
			}
		}
		if ( attackHitbox->GetActive() && player->GetPlayerHitBox()->GetActive() )
		{
			if ( Utils::OBB(attackHitbox->GetHitbox(), player->GetPlayerHitBox()->GetHitbox()) )
			{
				cout << "boss attack player" << endl;
				player->SetHp(damage * 2);
				getAttackTime = 0.f;
			}
		}
	}*/

	//position
	

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);

	//wall bound
	/*for ( const auto& hb : background->GetHitBoxList() )
	{
		if ( Utils::OBB(hb->GetHitbox(), bossHitbox->GetHitbox()) )
		{
			std::cout << "wall" << std::endl;
			SetBossPos();
		}
	}*/
}

void Enemy::Draw(RenderWindow& window)
{
	
	if ( GetActive() )
	{
		SpriteObject::Draw(window);
		window.draw(healthBar);
	}
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

void Enemy::SetBossPos()
{
	SetPos(prevPosition);
	healthBar.setPosition({ prevPosition.x, prevPosition.y - 15.f });
}

void Enemy::AttackPattern(float dt)
{
	moveTime -= dt;
	if ( moveTime <= 0.f )
	{
		SetState(States::Move);
		moveTime = 3.f;
	}
	//move
	if ( currState == States::Move )
	{
		dir = Utils::Normalize(player->GetPos() - GetPos());

		Translate(dir * this->speed * dt);
	}
	//attack motion
	if ( currState == States::Move && Utils::Distance(player->GetPos(), GetPos()) < 85.f )
	{
		//SetState(States::Attack);
	}

	//boss hits player
	hitTime += dt;
	
}