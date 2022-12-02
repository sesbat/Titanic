﻿ #include "Enemy.h"
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
#include "Ment.h"

Enemy::Enemy()
	: currState(States::None), speed(100.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), moveTime(15.f), hitTime(0.f), getAttackTime(1.f), attack(false), hp(500),
	maxHp(500), barScaleX(60.f), look(1.f, 0.f), isHit(false), type(1)
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
		((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position, this);
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
		Move(dt);
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
	if (InputMgr::GetKeyDown(Keyboard::F3))
	{
		//isMove = !isMove;
		playerPos = player->GetPos();
		movePos.clear();
		FindGrid();
		astar->AstarSearch(*isGreedObject, startPos, destPos);
		movePos = astar->GetCoordinate();
		SetState(States::Move);
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
	//dev
	/*if (isMove)
	{
		VertexArray lines(Quads, 4);
		if (!movePos.empty())
		{
			lines[0].position = { movePos.front().x - 30,movePos.front().y - 30.f };
			lines[1].position = { movePos.front().x + 30.f,movePos.front().y - 30.f };
			lines[2].position = { movePos.front().x + 30.f,movePos.front().y + 30.f };
			lines[3].position = { movePos.front().x - 30.f,movePos.front().y + 30.f };
		}
		window.draw(lines);
	}*/

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
	//move trigger
	isHit = true;
	moveTime = 0.f;
	playerPos = player->GetPos();
	movePos.clear();
	FindGrid();
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();

	hp -= num;
	if ( hp <= 0 )
	{
		hp = 0;
	}

	std::cout << "Enemy Add Ment" << std::endl;
	string ments[3] = { "Ouch..!", "Oh No!" , "Fuxx" };
	Vector2f randPos = { Utils::RandomRange(-20,20) + GetPos().x,
		Utils::RandomRange(-50,-40) + GetPos().y + 30.f };
	Ment* ment = new Ment();
	std::cout << "ment ptr : " << ment << std::endl;
	ment->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 30, Color::White, ments[Utils::RandomRange(0, 3)]);
	ment->SetAlways(false);
	ment->SetTimer(1.f);
	ment->SetOrigin(Origins::BC);
	ment->SetPos(randPos);
	ment->SetActive(true);
	SCENE_MGR->GetCurrScene()->AddGameObject(ment, LayerType::Object, 1);
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
		playerPos = player->GetPos();
		movePos.clear();
		FindGrid();
		astar->AstarSearch(*isGreedObject, startPos, destPos);
		movePos = astar->GetCoordinate();
	}

	if (!movePos.empty() && moveTime < 10.f && ((Utils::Distance(player->GetPos(), GetPos()) > 500.f) || isHit))
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
	if (movePos.empty())
	{
		//cout << "empty list1" << endl;
		SetState(States::Idle);
		Translate({ 0.f, 0.f });
		return;
	}

	Vector2f aPos = movePos.front();
	if ((Utils::Distance(aPos, GetPos()) <= 10.f))
	{
		if (movePos.empty())
		{
			//cout << "empty list2" << endl;
			SetState(States::Idle);
			return;
		}
		//cout << "in position" << endl;
		movePos.pop_front();
	}
	moveDir = Utils::Normalize(aPos - GetPos());

	prevPosition = GetPos();
	Translate(moveDir * this->speed * dt);

	//position
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}
	//wall bound
	Collision();
}

void Enemy::MoveToPos(float dt)
{
	if (movePos.empty())
	{
		//cout << "empty list1" << endl;
		SetState(States::Idle);
		Translate({ 0.f, 0.f });
		return;
	}
	
	Vector2f aPos = movePos.front();
	if ((Utils::Distance(aPos, GetPos()) <= 10.f))
	{
		if (movePos.empty())
		{
			//cout << "empty list2" << endl;
			SetState(States::Idle);
			return;
		}
		//cout << "in position" << endl;
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
	Collision();
}

void Enemy::Collision()
{
	//auto obj = scene->GetObjList();
	////wall bound
	//for (auto& objects : obj[LayerType::Object][0])
	//{
	//	auto hit = ((HitBoxObject*)objects)->GetBottom();
	//	if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
	//		continue;
	//	if (objects->GetName() == "TREE" ||
	//		objects->GetName() == "STONE" ||
	//		objects->GetName() == "BLOCK" ||
	//		objects->GetName() == "PLAYER")
	//	{
	//		if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
	//		{
	//			SetEnemyPos();
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
					SetEnemyPos();
			}
		}
	}
}

void Enemy::FindGrid()
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;
	//cout << "start pos" << startPos.first << " " << startPos.second << endl;
	
	//Enemy dest pos
	destPos.first = (int)player->GetPlayerBottom().x / 60;
	destPos.second = (int)player->GetPlayerBottom().y / 60;
	//cout << "dest pos" << destPos.first << " " << destPos.second << endl;
}
