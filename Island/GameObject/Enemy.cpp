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
#include "../Ui/GameSceneUiMgr.h"
#include "../Scens/GameScene.h"
#include "../Astar/Astar.h"
#include <iostream>
#include <stack>

Enemy::Enemy()
	: currState(States::None), speed(100.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), moveTime(0.f), hitTime(0.f), getAttackTime(1.f), attack(false), hp(15),
	maxHp(15), barScaleX(60.f), look(1.f, 0.f), isHit(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Player* player, int type)
{
	HitBoxObject::Init();
	this->player = player;

	hp = maxHp;
	switch (type)
	{
	case 1:
		gun = new Gun(GunType::Shotgun, User::Enemy);
		break;
	case 2:
		gun = new Gun(GunType::Rifle, User::Enemy);
		break;
	case 3:
		gun = new Gun(GunType::Sniper, User::Enemy);
		break;
	}
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

	astar = new Astar();
}

void Enemy::SetState(States newState)
{
	if (currState == newState)
		return;

	currState = newState;
	
	switch ( currState )
	{
	case Enemy::States::Idle:
		animator.Play((direction.x > 0.f) ? "EnemyIdle" : "EnemyIdleLeft");
		break;
	case Enemy::States::Move:
		animator.Play((direction.x > 0.f) ? "EnemyMove" : "EnemyMoveLeft");
		break;
	case Enemy::States::Dead:
		((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position);
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
	if (Utils::Distance(player->GetPos(), GetPos()) < 500.f)
	{
		//look = player->GetPos();
		lookDir = Utils::Normalize(player->GetPos() - GetPos());
		direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
	}
	else
	{
		lookDir = direction;
	}
	
	//enemy dead
	if (hp <= 0)
	{
		SetState(States::Dead);
	}

	//enemy attack
	if (currState != States::Dead)
	{
		AttackPattern(dt);
	}
	

	//move
	if (currState == States::Move)
	{
		MoveToPos(dt);
		//Move(dt);
	}
	
	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);
	
	//position
	bottomPos = bottom->GetHitBottomPos();

	//gun
	gun->Update(dt);

	//dev input
	if (InputMgr::GetKeyDown(Keyboard::F2))
	{
		isHit = !isHit;
	}
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
	isHit = true;
	moveTime = 0.f;
	attack = true;
	playerPos = player->GetPos();
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
	if (hitTime >= 0.8f && (Utils::Distance(player->GetPos(), GetPos()) < 500.f))
	{
		gun->Fire(GetPos(), false);
		hitTime = 0.f;
		moveTime = 0.f;
		attack = true;
		playerPos = player->GetPos();
		movePos.clear();
		FindGrid();

		astar->AstarSearch(*isGreedObject, startPos, destPos);
		movePos = astar->GetCoordinate();
	}

	if (attack && moveTime < 10.f && ((Utils::Distance(player->GetPos(), GetPos()) > 500.f) || isHit))
	{
		SetState(States::Move);
	}
	else
	{
		SetState(States::Idle);
		isHit = false;
	}
	
	//timer
	hitTime += dt;
	moveTime += dt;
	
}

void Enemy::Move(float dt)
{
	if (!movePos.empty())
	{
		Vector2f pos = movePos.back();
		while (GetPos() != pos)
		{
			moveDir = Utils::Normalize(pos - GetPos());

			//x dir
			prevPosition = GetPos();
			Translate({ moveDir.x * this->speed * dt, 0.f });
			//position
			for (auto& hit : hitboxs)
			{
				hit->SetPos(GetPos());
			}
			//wall bound
			Collision();

			//y dir
			prevPosition = GetPos();
			Translate({ 0.f,  moveDir.y * this->speed * dt });
			//position
			for (auto& hit : hitboxs)
			{
				hit->SetPos(GetPos());
			}
			Collision();
		}
		movePos.pop_back();
	}
	
}

void Enemy::MoveToPos(float dt)
{
	if (movePos.empty())
	{
		cout << "empty list" << endl;
		SetState(States::Idle);
		Translate({ 0.f, 0.f });
		return;
	}
	
	Vector2f aPos = movePos.front();
	if ((Utils::Distance(aPos, GetPos()) <= 10.f))
	{
		if (movePos.empty())
		{
			cout << "empty list" << endl;
			SetState(States::Idle);
			return;
		}
		cout << "in position" << endl;
		movePos.pop_front();
	}
	moveDir = Utils::Normalize(aPos - GetPos());

	prevPosition = GetPos();
	Translate( moveDir * this->speed * dt );

	//position
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
	//wall bound
	//Collision();
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
		if (objects->GetName() == "STONE" ||
			objects->GetName() == "BLOCK")
		{
			if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
			{
				SetEnemyPos();
				break;
			}
		}
	}
}

void Enemy::FindGrid()
{
	startPos.first = (int)bottomPos.x / 60;//(int)GetPos().x / 60;
	startPos.second = (int)bottomPos.y / 60;//(int)GetPos().y / 60;
	cout << "start pos" << startPos.first << " " << startPos.second << endl;

	destPos.first = (int)player->GetPlayerBottom().x / 60;//(int)playerPos.x / 60;
	destPos.second = (int)player->GetPlayerBottom().y / 60;//(int)playerPos.y / 60;
	cout << "dest pos" << destPos.first << " " << destPos.second << endl;
}
