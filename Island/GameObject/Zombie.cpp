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
	isHit(false), isInSight(true)
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
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigIdle"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigIdleLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigMove"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigMoveLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigStun"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigStunLeft"));

	dashHitbox = new HitBox();
	dashHitbox->SetFillColor(Color::Red);
	dashHitbox->SetPos({ GetPos().x - 100.f,GetPos().y - 60.f });
	dashHitbox->SetHitbox({ 0.f,0.f,200.f,100.f });

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
		animator.Play((direction.x > 0.f) ? "BossBigIdle" : "BossBigIdleLeft");
		break;
	case Zombie::States::Move:
	case Zombie::States::Dash:
		animator.Play((direction.x > 0.f) ? "BossBigMove" : "BossBigMoveLeft");
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
}

void Zombie::Draw(RenderWindow& window)
{
}

void Zombie::OnCompleteDead()
{
}

bool Zombie::EqualFloat(float a, float b)
{
    return false;
}

void Zombie::SetHp(int num)
{
}

void Zombie::SetHpBar()
{
}

void Zombie::SetZombiePos()
{
}

void Zombie::AttackPattern(float dt)
{
}

void Zombie::Move(float dt)
{
}

void Zombie::Collision()
{
}

void Zombie::ContactDamage()
{
}

void Zombie::SetDashPos()
{
}

void Zombie::FindGrid()
{
}

void Zombie::FindGrid(Vector2f pos)
{
}

void Zombie::CheckIsInSight()
{
}
