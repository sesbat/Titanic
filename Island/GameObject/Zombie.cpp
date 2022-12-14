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
	:maxSpeed(100)
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

	bottomPos = bottom->GetHitBottomPos();

	astar = new Astar();

	MakePath();
	movePos.clear();

	//SetTexture(*RESOURCES_MGR->GetTexture(enemyType));
	SetOrigin(Origins::BC);
	SetState(States::Idle);
	animator.Play((direction.x > 0.f) ? "ZombileIdle" : "ZombileIdleLeft");
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

	//enemy attack
	if (curState != States::Dead && curState == States::Idle)
	{
		cout << "move" << endl;
		PatrolPattern(dt);
		/*if (patrolTime <= 0.f && !attack)
		{
			cout << "move" << endl;
			PatrolPattern(dt);
			patrolTime = 5.f;
		}
		if (curState == States::Idle)
		{
			patrolTime -= dt;
		}*/
		//AttackPattern(dt);
	}

	//move
	if (curState == States::Move)
	{
		Move(dt);

	}

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);

	//position
	bottomPos = bottom->GetHitBottomPos();



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
	if (GetActive() && isInSight)
	{
		HitBoxObject::Draw(window);
		window.draw(healthBar);
		SetColor(Color::White);
		
	}
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
		SetColor(Color::White);
		if (!isdead)
			window.draw(healthBar);
		lines[0].position = { GetPos() };
		lines[1].position = { player->GetPos() };
		window.draw(lines);
	}
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
		((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position, this);
		SOUND_MGR->Play("sounds/death.wav");
		SetActive(false);
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
		cout << "empty list1" << endl;
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

//void Zombie::AttackPattern(float dt)
//{
//	CheckIsInSight();
//	
//	//attack motion
//	if (hitTime >= 0.8f )
//	{
//		if (!player->GetHide() && (Utils::Distance(player->GetPos(), GetPos()) < 500.f) && isInSight)
//		{
//			lookDir = Utils::Normalize(player->GetPos() - GetPos());
//			direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
//			SetState(States::Idle);
//			animator.Play((direction.x > 0.f) ? "EnemyIdle" : "EnemyIdleLeft");
//			gun->SetLookDir(lookDir);
//			gun->Fire(GetPos(), false);
//			HideStop();
//			hitTime = 0.f;
//			moveTime = 0.f;
//			playerPos = player->GetPos();
//			movePos.clear();
//			FindGrid();
//			astar->AstarSearch(*isGreedObject, startPos, destPos);
//			movePos = astar->GetCoordinate();
//			attack = true;
//		}
//	}
//	//cout << movePos.empty() << endl;
//
//	if ((!movePos.empty() && moveTime < initMoveTime && (Utils::Distance(player->GetPos(), GetPos()) > 500.f) || !isInSight) || (isHit || isSearch))
//	{
//		SetState(States::Move);
//		if (isHit)
//		{
//			CallFriends();
//		}
//		//direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
//	}
//	else
//	{
//		//SetState(States::Idle);
//		isHit = false;
//		isSearch = false;
//		attack = false;
//
//	}
//
//	//timer
//	hitTime += dt;
//	moveTime += dt;
//}

void Zombie::PatrolPattern(float dt)
{
	MakePath();
	//int num = Utils::RandomRange(0, 5);
	//Vector2f point = patrolPos[num];
	movePos.clear();
	FindGrid(patrolPos);
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();
	SetState(States::Move);
}

//void Zombie::FindGrid()
//{
//	//Enemy start pos
//	startPos.first = (int)bottomPos.x / 60;
//	startPos.second = (int)bottomPos.y / 60;
//
//	//Enemy dest pos
//	destPos.first = (int)player->GetPlayerBottom().x / 60;
//	destPos.second = (int)player->GetPlayerBottom().y / 60;
//}
//
//void Zombie::FindGrid(Vector2f pos)
//{
//	//Enemy start pos
//	startPos.first = (int)bottomPos.x / 60;
//	startPos.second = (int)bottomPos.y / 60;
//
//	//Enemy dest pos
//	destPos.first = (int)pos.x / 60;
//	destPos.second = (int)pos.y / 60;
//}
//
//void Zombie::CheckIsInSight()
//{
//	VertexArray lines(LineStrip, 2);
//	lines[0].position = { GetPos() };
//	lines[1].position = { player->GetPos() };
//
//	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
//	{
//		auto boundInObj = ((GameScene*)scene)->ObjListObb(lines.getBounds());
//
//		for (auto& obj : boundInObj)
//		{
//			if (Utils::LineRect(
//				GetPos(),
//				player->GetPos(),
//				obj->GetBottom()->GetHitbox()))
//			{
//				if (obj->GetName() == "TREE" ||
//					obj->GetName() == "STONE" ||
//					obj->GetName() == "BLOCK")
//				{
//					isInSight = false;
//					break;
//				}
//			}
//			else
//			{
//				isInSight = true;
//			}
//
//		}
//	}
//}
//
//void Zombie::MakePath()
//{
//	int x, y;
//	//int num = 0;
//	while (1)
//	{
//		if (Utils::RandomRange(0, 2) == 0)
//		{
//			x = ((int)bottomPos.x / 60) + Utils::RandomRange(0, patrolBlock);
//			y = ((int)bottomPos.y / 60) + Utils::RandomRange(0, patrolBlock);
//		}
//		else
//		{
//			x = ((int)bottomPos.x / 60) + Utils::RandomRange(-patrolBlock, 0);
//			y = ((int)bottomPos.y / 60) + Utils::RandomRange(-patrolBlock, 0);
//		}
//		if (x > 0 && y > 0 && y < 72 && x < 128)
//		{
//			if (!CheckWall(x, y))
//			{
//				patrolPos = { x * 60.f,y * 60.f };
//				break;
//
//				//cout << "path x: " << x << " y: " << y << endl;
//			}
//		}
//
//	}
//	return;
//}
//
//bool Zombie::CheckWall(int x, int y)
//{
//	return (*isGreedObject)[y][x];
//}
//
//
//void Zombie::CallFriends()
//{
//}

void Zombie::SetHp(int num)
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
		isdead = true;
	}
}
