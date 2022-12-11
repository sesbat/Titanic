#include "Boss.h"
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

Boss::Boss()
	: currState(States::None), type(Type::None),
	speed(100.f), direction(1.f, 0.f), lastDirection(1.f, 0.f),
	moveTime(15.f), hitTime(0.f), getAttackTime(1.f),
	hp(500), maxHp(500), barScaleX(60.f), look(1.f, 0.f),
	isHit(false), isInSight(true), attack(false), isSearch(false), isStart(false)
{
}

Boss::~Boss()
{
    Release();
}

void Boss::Release()
{
    if (gun != nullptr)
        delete gun;
    gun = nullptr;

    if (astar != nullptr)
        delete astar;
    astar = nullptr;


    HitBoxObject::Release();
}

void Boss::Init(Player* player)
{
	HitBoxObject::Init();
	this->player = player;

	hp = maxHp;
	type = Type::Big;

	switch (type)
	{
	case Boss::Type::Big:
		animator.SetTarget(&sprite);
		//animation
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigIdle"));
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigIdleLeft"));
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigMove"));
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigMoveLeft"));
		
		//gun = new Gun(GunType::Shotgun, User::Enemy);
		//gun->SetEnemy(this);
		//gun->Init();
		break;
	case Boss::Type::Kiba:
		break;
	default:
		break;
	}
	/*switch (type)
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
	gun->Init();*/

	
	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	scene = SCENE_MGR->GetCurrScene();

	astar = new Astar();

	//bottomPos = bottom->GetHitBottomPos();
	movePos.clear();
	SetState(States::Idle);
}

void Boss::SetState(States newState)
{
	if (currState == newState)
		return;

	currState = newState;
	//lastDirection = direction;

	switch (currState)
	{
	case Boss::States::Idle:
		animator.Play((direction.x > 0.f) ? "BossBigIdle" : "BossBigIdleLeft");
		break;
	case Boss::States::Move:
		animator.Play((direction.x > 0.f) ? "BossBigMove" : "BossBigMoveLeft");
		break;
	case Boss::States::Dead:
		//((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position, this);
		SOUND_MGR->Play("sounds/death.wav");
		SetActive(false);
		break;
	}
}

Boss::States Boss::GetState()
{
    return currState;
}

void Boss::Update(float dt)
{
	/*if (!enabled || !IsInView())
		return;*/

	HitBoxObject::Update(dt);

	//ready
	//if got shot from range will be problem
	//need change
	/*if (isStart == false && (Utils::Distance(player->GetPos(), GetPos()) < 1000.f))
	{
		isStart = true;
	}*/

	//start fight
	if (!isStart)
	{
		if (Utils::Distance(player->GetPos(), GetPos()) < 1000.f)
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
		if (currState != States::Dead)
		{
			
			//AttackPattern(dt);
		}

		//move
		if (currState == States::Move)
		{
			//Move(dt);

		}

		//hp bar
		SetHpBar();

		//animation
		animator.Update(dt);

		//position
		//bottomPos = bottom->GetHitBottomPos();

		//gun
		//gun->Update(dt);

		//dev
		if (InputMgr::GetKeyDown(Keyboard::Z))
		{
			SetState(States::Move);
		}
		if (InputMgr::GetKeyDown(Keyboard::X))
		{
			SetState(States::Idle);
		}
	}

	

}

void Boss::Draw(RenderWindow& window)
{
	/*if (!enabled || !IsInView())
		return;*/
	if (GetActive())
	{
		HitBoxObject::Draw(window);
		window.draw(healthBar);
	}
	/*if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}*/
	//gun->Draw(window);

}

void Boss::OnCompleteDead()
{
	SetActive(false);
}

bool Boss::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Boss::SetHp(int num)
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
	if (hp <= 0)
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

void Boss::SetHpBar()
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

void Boss::SetBossPos()
{
	SetPos(prevPosition);

	for (auto& hit : hitboxs)
	{
		hit->SetPos(prevPosition);
	}
	bottom->SetPos(prevPosition);

	healthBar.setPosition({ prevPosition.x, prevPosition.y - 35.f });
}

void Boss::AttackPattern(float dt)
{
	CheckIsInSight();
	CheckIsInWall();
	//attack motion
	if (hitTime >= 0.8f && gun->GetIsInWall())
	{
		if ((Utils::Distance(player->GetPos(), GetPos()) < 500.f) && isInSight)
		{
			lookDir = Utils::Normalize(player->GetPos() - GetPos());
			direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
			SetState(States::Idle);
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
			attack = true;
		}
	}
	//cout << movePos.empty() << endl;

	if ((!movePos.empty() && moveTime < 10.f && (Utils::Distance(player->GetPos(), GetPos()) > 500.f) || !isInSight) || isHit)
	{
		SetState(States::Move);
		
		//direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
	}
	else
	{
		//SetState(States::Idle);
		isHit = false;
		isSearch = false;
		attack = false;

	}

	//timer
	hitTime += dt;
	moveTime += dt;
}

void Boss::Move(float dt)
{
	if (movePos.empty())
	{
		//cout << "empty list1" << endl;
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

void Boss::Collision()
{
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
					SetBossPos();
			}
		}
	}
}

void Boss::FindGrid()
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;

	//Enemy dest pos
	destPos.first = (int)player->GetPlayerBottom().x / 60;
	destPos.second = (int)player->GetPlayerBottom().y / 60;
}

void Boss::FindGrid(Vector2f pos)
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;

	//Enemy dest pos
	destPos.first = (int)pos.x / 60;
	destPos.second = (int)pos.y / 60;
}

void Boss::CheckIsInWall()
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

void Boss::CheckIsInSight()
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

void Boss::MakePath()
{
	//int x, y;
	////int num = 0;
	//while (1)
	//{
	//	if (Utils::RandomRange(0, 2) == 0)
	//	{
	//		x = ((int)bottomPos.x / 60) + Utils::RandomRange(0, patrolBlock);
	//		y = ((int)bottomPos.y / 60) + Utils::RandomRange(0, patrolBlock);
	//	}
	//	else
	//	{
	//		x = ((int)bottomPos.x / 60) + Utils::RandomRange(-patrolBlock, 0);
	//		y = ((int)bottomPos.y / 60) + Utils::RandomRange(-patrolBlock, 0);
	//	}
	//	if (x > 0 && y > 0 && y < 72 && x < 128)
	//	{
	//		if (!CheckWall(x, y))
	//		{
	//			patrolPos = { x * 60.f,y * 60.f };
	//			return;
	//			//cout << "path x: " << x << " y: " << y << endl;
	//		}
	//	}

	//}
}

bool Boss::CheckWall(int x, int y)
{
	return (*isGreedObject)[y][x];
}
