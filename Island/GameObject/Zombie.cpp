#include "Zombie.h"
#include "Player.h"
#include "HitBox.h"
#include "Object.h"
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

Zombie::Zombie()
	: currState(States::None),
	speed(100.f), maxSpeed(100), dashSpeed(110.f),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	timer(0.f), moveTime(2.f), stopTime(1.f), hitTime(1.f), hitTimer(0.f), 
	hp(1500), maxHp(1500), barScaleX(60.f), 
	look(1.f, 0.f), 
	damage(10),
	isHit(false), isInSight(true), attack(true),isSearch(false), patrolBlock(10)
{
}

Zombie::~Zombie()
{
	Release();
}

void Zombie::Release()
{
	if (astar != nullptr)
		delete astar;
	astar = nullptr;

	HitBoxObject::Release();
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

	dashHitbox = new HitBox();
	dashHitbox->SetFillColor(Color::Red);
	dashHitbox->SetPos({ GetPos().x - 100.f,GetPos().y - 60.f });
	dashHitbox->SetHitbox({ 0.f,0.f,50.f,100.f });

	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ barScaleX, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	scene = SCENE_MGR->GetCurrScene();

	astar = new Astar();

	bottomPos = bottom->GetHitBottomPos();
	movePos.clear();
	SetState(States::Idle);
}

void Zombie::SetState(States newState)
{
	if (currState == newState)
		return;

	currState = newState;
	//lastDirection = direction;

	switch (currState)
	{
	case Zombie::States::Idle:
		animator.Play((direction.x > 0.f) ? "ZombileIdle" : "ZombileIdleLeft");
		break;
	case Zombie::States::Move:
		animator.Play((direction.x > 0.f) ? "ZombileMove" : "ZombileMoveLeft");
		break;
	case Zombie::States::Dead:
		//((GameScene*)(SCENE_MGR->GetCurrScene()))->SetDeadEnemy(items, position, this);
		SOUND_MGR->Play("sounds/death.wav");
		SetActive(false);
		break;
	}
}

Zombie::States Zombie::GetState()
{
    return currState;
}

void Zombie::Update(float dt)
{
	if (!enabled || !IsInView())
		return;

	HitBoxObject::Update(dt);

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
	if (currState != States::Dead)
	{
		if (patrolTime <= 0.f && !attack)
		{
			PatrolPattern(dt);
			patrolTime = 5.f;
		}
		if (currState == States::Idle)
		{
			patrolTime -= dt;
		}
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

	//dev
	VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		lines[0].position = { GetPos() };
		lines[1].position = { player->GetPos() };
		window.draw(lines);
	}
}

void Zombie::OnCompleteDead()
{
	SetActive(false);
}

bool Zombie::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Zombie::SetHp(int num)
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
}

void Zombie::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	healthBar.setSize({ (barScaleX / maxHp) * hp, 15.f });
	if (hp > (maxHp / 2))
	{
		healthBar.setFillColor(Color::Green);
	}
	else if (hp <= (maxHp / 2) && hp > (maxHp / 5))
	{
		healthBar.setFillColor(Color::Yellow);
	}
	else
	{
		healthBar.setFillColor(Color::Red);
	}
	if (hp <= 0)
	{
		healthBar.setOutlineThickness(0.f);
	}
	else
	{
		healthBar.setOutlineThickness(2.f);
	}
}

void Zombie::SetZombiePos()
{
	SetPos(prevPosition);

	for (auto& hit : hitboxs)
	{
		hit->SetPos(prevPosition);
	}
	bottom->SetPos(prevPosition);

	healthBar.setPosition({ prevPosition.x, prevPosition.y - 35.f });
}

void Zombie::AttackPattern(float dt)
{
}

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

void Zombie::Move(float dt)
{
	if (movePos.empty())
	{
		//cout << "empty list1" << endl;
		SetState(States::Idle);
		Translate({ 0.f, 0.f });

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

void Zombie::Collision()
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
					obj->GetName() == "RADIATION" ||
					obj->GetName() == "INVISIBLE")
				{
					SetZombiePos();
					
				}
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

void Zombie::FindGrid()
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;

	//Enemy dest pos
	destPos.first = (int)player->GetPlayerBottom().x / 60;
	destPos.second = (int)player->GetPlayerBottom().y / 60;
}

void Zombie::FindGrid(Vector2f pos)
{
	//Enemy start pos
	startPos.first = (int)bottomPos.x / 60;
	startPos.second = (int)bottomPos.y / 60;

	//Enemy dest pos
	destPos.first = (int)pos.x / 60;
	destPos.second = (int)pos.y / 60;
}

void Zombie::CheckIsInSight()
{
	VertexArray lines(LineStrip, 2);
	lines[0].position = { GetPos().x,GetPos().y + 20.f };
	lines[1].position = { player->GetPos().x,player->GetPos().y + 20.f };

	if (SCENE_MGR->GetCurrSceneType() == Scenes::GameScene)
	{
		auto boundInObj = ((GameScene*)scene)->ObjListObb(lines.getBounds());

		for (auto& obj : boundInObj)
		{
			if (Utils::LineRect(
				{ GetPos().x,GetPos().y + 20.f },
				{ player->GetPos().x,player->GetPos().y + 20.f },
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

void Zombie::MakePath()
{
}

bool Zombie::CheckWall(int x, int y)
{
	return false;
}

void Zombie::SetIsSearch(bool hit)
{
}

void Zombie::CallFriends()
{
}
