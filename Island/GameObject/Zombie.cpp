#include "Zombie.h"
#include "Player.h"
#include "../Astar/Astar.h"
#include "../Scens/SceneManager.h"
#include "../Scens/GameScene.h"
#include "../GameObject/HitBox.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/SoundManager.h"	
#include <iostream>

Zombie::Zombie()
	:maxSpeed(80.f), dashSpeed(150.f), hitTime(1.f), hitTimer(0.f), damage(10)
{
}

Zombie::~Zombie()
{
	Enemy::Release();
	Release();
}

void Zombie::Init(Player* player)
{
	HitBoxObject::Init();
	this->player = player;
	
	hp = maxHp;
	speed = maxSpeed;

	animator.SetTarget(&sprite);
	//animation
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("ZombileIdle"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("ZombileIdleLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("ZombileMove"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("ZombileMoveLeft"));

	hp = maxHp;// = datamaxHp;

	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 75.f });

	Utils::SetOrigin(healthBar, Origins::MC);

	scene = SCENE_MGR->GetCurrScene();

	dashHitbox = new HitBox();
	dashHitbox->SetFillColor(Color::Yellow);
	dashHitbox->SetPos({ GetPos().x - 25.f,GetPos().y - 30.f });
	dashHitbox->SetHitbox({ 0.f,0.f,45.f,60.f });

	astar = new Astar();

	bottomPos = bottom->GetHitBottomPos();

	MakePath();
	movePos.clear();
	SetState(States::Idle);
}

void Zombie::Update(float dt)
{
	if (!enabled || !IsInView())
		return;

	HitBoxObject::Update(dt);
	HideUpdate(dt);
	CheckIsInSight();
	if (!player->GetHide() && isInSight && (Utils::Distance(player->GetPos(), GetPos()) < 500.f))
	{
		//look = player->GetPos();
		lookDir = Utils::Normalize(player->GetPos() - GetPos());
		direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
		lastDirection = direction;
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
	if (((Utils::Distance(player->GetPos(), GetPos()) < searchDis) && isInSight))
	{
		isHit = true;
		
	}
	//enemy attack
	if (curState == States::Idle && !isHit)
	{
		PatrolPattern(dt);
		
	}
	if (isHit)
	{
		AttackPattern(dt);
		
	}
	//move
	if (curState == States::Move)
	{
		Move(dt);

	}

	ContactDamage();
	//contact hit timer
	hitTimer -= dt;

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);

	//position
	bottomPos = bottom->GetHitBottomPos();
	dashHitbox->SetPos({ GetPos().x - 25.f,GetPos().y - 30.f });


	auto boundInObj = ((GameScene*)scene)->ObjListObb(this);

	bool isNowHide = false;
	for (auto& obj : boundInObj)
	{
		if (obj->GetName() == "BUSH" && Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
		{
			isNowHide = true;
			SetHide(true);
			break;
		}
	}

	if (!isNowHide)
		SetHide(false);
}

void Zombie::Draw(RenderWindow& window)
{
	if (!enabled || !IsInView())
		return;
	//dashHitbox->Draw(window);
	if (GetActive() && isInSight)
	{
		HitBoxObject::Draw(window);
		//window.draw(healthBar);
		SetColor(Color::White);
		
	}
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}
	/*VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		HitBoxObject::Draw(window);
		SetColor(Color::White);
		if (!isdead)
			window.draw(healthBar);
		lines[0].position = { GetPos() };
		lines[1].position = { player->GetPos() };
		window.draw(lines);
	}*/
}

void Zombie::Release()
{
	Enemy::Release();
}

void Zombie::SetState(States newState)
{
	if (curState == newState)
		return;

	curState = newState;
	//lastDirection = direction;

	switch (curState)
	{
	case Zombie::States::Idle:
		animator.Play((direction.x > 0.f) ? "ZombileIdle" : "ZombileIdleLeft");
		break;
	case Zombie::States::Move:
		animator.Play((direction.x > 0.f) ? "ZombileMove" : "ZombileMoveLeft");
		break;
	case Zombie::States::Dead:
	{
		((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position, this, "graphics/zombieDead.png");
		SOUND_MGR->Play("sounds/death.wav");
		SetActive(false);
	}
	break;
	}
}

Zombie::States Zombie::GetState()
{
	return curState;
}

void Zombie::Move(float dt)
{
	if (movePos.empty())
	{
		SetState(States::Idle);
		Translate({ 0.f, 0.f });
		isHit = false;
		isSearch = false;
		//patrolTime = 5.f;
		return;
	}

	Vector2f aPos = movePos.front();
	direction.x = (aPos.x > GetPos().x) ? 1.f : -1.f;
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

void Zombie::AttackPattern(float dt)
{
	speed = dashSpeed;
	MakePath();
	movePos.clear();
	FindGrid();
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();
	CallFriends();
	SetState(States::Move);
}

void Zombie::PatrolPattern(float dt)
{
	MakePath();
	movePos.clear();
	FindGrid(patrolPos);
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();
	isHit = false;
	SetState(States::Move);
}

void Zombie::CallFriends()
{
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(this);
		auto* enemyfriend = ((GameScene*)scene)->GetEnemyList();
		if (enemyfriend->size() <= 1)
		{
			return;
		}

		for (auto obj = enemyfriend->begin(); obj != enemyfriend->end(); obj++)
		{
			if ((Utils::Distance((*obj)->GetPos(), GetPos()) < searchDis))
			{
				(*obj)->SetIsHit(true);
			}
		}

	}
}

void Zombie::ContactDamage()
{
	for (auto& pHit : player->GetHitBoxs())
	{
		if (dashHitbox->GetActive() && Utils::OBB(dashHitbox->GetHitbox(), pHit->GetHitbox()))
		{
			if (hitTimer <= 0)
			{
				player->SetHp(damage);
				hitTimer = hitTime;
				//cout << "damage" << endl;
			}
			break;
		}
	}
}

void Zombie::SetHp(int num)
{
	isHit = true;
	moveTime = 0.f;
	playerPos = player->GetPos();
	movePos.clear();
	FindGrid();
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();

	hp -= num;
	if (hp <= 0)
	{
		hp = 0;
	}
}

