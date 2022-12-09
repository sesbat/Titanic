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
#include "Ment.h"

Enemy::Enemy()
	: currState(States::None), speed(100.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), moveTime(15.f), hitTime(0.f), getAttackTime(1.f), patrolTime(5.f), hp(500),
	maxHp(500), barScaleX(60.f), look(1.f, 0.f), isHit(false), type(1), isInSight(true)
{
	hideDelay = 1.f;
}

Enemy::~Enemy()
{
	Release();
}

void Enemy::Release()
{

	if (gun != nullptr)
		delete gun;
	gun = nullptr;

	if (astar != nullptr)
		delete astar;
	astar = nullptr;


	HitBoxObject::Release();
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
	MakePath();
}

void Enemy::SetState(States newState)
{
	if (currState == newState)
		return;

	currState = newState;
	//lastDirection = direction;

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
		SOUND_MGR->Play("sounds/death.wav");
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
	HideUpdate(dt);

	if (isInSight && Utils::Distance(player->GetPos(), GetPos()) < 500.f)
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
	/*if (movePos.empty())
	{
		patrolTime -= dt;
		SetState(States::Idle);
	}
	*/
	//enemy attack
	if (currState != States::Dead)
	{
		/*if (patrolTime <= 0.f)
		{
			PatrolPattern(dt);
			patrolTime = 5.f;
		}*/
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
	VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		lines[0].position = { GetPos() };
		lines[1].position = { player->GetPos() };
		window.draw(lines);
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


	string ments[3] = { "Ouch..!", "Oh No!" , "Fuxx" };
	Vector2f randPos = { Utils::RandomRange(-20,20) + GetPos().x,
		Utils::RandomRange(-50,-40) + GetPos().y + 30.f };
	Ment* ment = new Ment();
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
	if (hp > (maxHp / 2))
	{
		if (!isHide)
			healthBar.setFillColor(Color::Green);
	}
	else if (hp <= (maxHp / 2) && hp > (maxHp / 5))
	{
		if (!isHide)
			healthBar.setFillColor(Color::Yellow);
	}
	else
	{
		if (!isHide)
			healthBar.setFillColor(Color::Red);
	}
	if (hp <= 0)
	{
		if (!isHide)
			healthBar.setOutlineThickness(0.f);
	}
	else
	{
		if (!isHide)
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
	CheckIsInSight();
	CheckIsInWall();
	//attack motion
	if (hitTime >= 0.8f &&  gun->GetIsInWall() )
	{
		if ((Utils::Distance(player->GetPos(), GetPos()) < 500.f) && isInSight)
		{
			lookDir = Utils::Normalize(player->GetPos() - GetPos());
			direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
			animator.Play((direction.x > 0.f) ? "EnemyIdle" : "EnemyIdleLeft");
			gun->SetLookDir(lookDir);
			gun->Fire(GetPos(), false);
			hitTime = 0.f;
			moveTime = 0.f;
			playerPos = player->GetPos();
			movePos.clear();
			FindGrid();
			astar->AstarSearch(*isGreedObject, startPos, destPos);
			movePos = astar->GetCoordinate();
		}
	}
	//cout << movePos.empty() << endl;

	if ((!movePos.empty() && moveTime < 10.f && (Utils::Distance(player->GetPos(), GetPos()) > 500.f) || !isInSight) || isHit)
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

void Enemy::PatrolPattern(float dt)
{
	int num = Utils::RandomRange(0, 5);
	Vector2f point = patrolPos[num];
	movePos.clear();
	FindGrid(point);
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();
	SetState(States::Move);
}

void Enemy::Move(float dt)
{
	if (movePos.empty())
	{
		//cout << "empty list1" << endl;
		SetState(States::Idle);
		Translate({ 0.f, 0.f });
		isHit = false;
		
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

		for (auto& obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), bottom->GetHitbox()))
			{
				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "RADIATION")
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
	
	//Enemy dest pos
	destPos.first = (int)player->GetPlayerBottom().x / 60;
	destPos.second = (int)player->GetPlayerBottom().y / 60;
	
}

void Enemy::FindGrid(Vector2f pos)
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;
	
	//Enemy dest pos
	destPos.first = (int)pos.x / 60;
	destPos.second = (int)pos.y / 60;
	
}

void Enemy::CheckIsInWall()
{
	//gun fire
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(this);

		for (auto& obj : boundInObj)
		{
			if (Utils::OBB(obj->GetBottom()->GetHitbox(), gun->GetHitbox()))
			{

				if (obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK" ||
					obj->GetName() == "RADIATION")
				{
					//cout << "wall" << endl;
					gun->SetIsInWall(false);
					break;
				}
			}
			else
			{
				//cout << "not wall" << endl;
				gun->SetIsInWall(true);
			}
		}
	}
}

void Enemy::CheckIsInSight()
{
	VertexArray lines(LineStrip, 2);
	lines[0].position = { GetPos() };
	lines[1].position = { player->GetPos() };
	
	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(lines.getBounds());

		for (auto& obj : boundInObj)
		{
			if (Utils::LineRect(
				GetPos(),
				player->GetPos(),
				obj->GetBottom()->GetHitbox()))
			{
				if (obj->GetName() == "TREE" ||
					obj->GetName() == "STONE" ||
					obj->GetName() == "BLOCK")
				{
					isInSight = false;
					break;
				}
			}
			else
			{
				isInSight = true;
			}
			
		}
	}
}

void Enemy::MakePath()
{
	float x, y;

	for (int i = 0; i < 5; i++)
	{
		if (Utils::RandomRange(0, 2) == 0)
		{
			x = GetPos().x + Utils::RandomRange(100.f, 300.f);
			y = GetPos().y + Utils::RandomRange(100.f, 300.f);
		}
		else
		{
			x = GetPos().x + Utils::RandomRange(-100.f, -300.f);
			y = GetPos().y + Utils::RandomRange(-100.f, -300.f);
		}
		patrolPos.push_back({ x,y });
		//cout << "path x: " << x << " y: " << y << endl;
		
	}
	
}

bool Enemy::GetHide()
{
	return isHide;
}

void Enemy::SetHide(bool state)
{
	if (isHide && !state)
	{
		SetColor(Color::White);
		gun->SetColor(Color::White);
		healthBar.setFillColor(Color::White);
		SetHpBar();
		hideDelayTimer = 0.f;
		isHitBullet = false;
	}
	if (isHitBullet)
	{
		return;
	}
	if (!isHide && state)
	{
		SetColor(Color(0, 0, 0, 0));
		gun->SetColor(Color(0, 0, 0, 0));
		healthBar.setFillColor(Color(0, 0, 0, 0));
		healthBar.setOutlineThickness(0.f);

	}
	isHide = state;
}

void Enemy::HideUpdate(float dt)
{
	if (isHitBullet)
	{
		hideDelayTimer += dt;
		if (hideDelayTimer > hideDelay)
		{
			hideDelayTimer = 0.f;
			isHitBullet = false;
			gun->SetColor(Color::White);
			SetColor(Color::White);
			healthBar.setFillColor(Color::White);
		}
	}
}

void Enemy::HideStop()
{
	isHitBullet = true; hideDelayTimer = 0.f; isHide = false;

	SetColor(Color::White);
	gun->SetColor(Color::White);
	healthBar.setFillColor(Color::White);

}
