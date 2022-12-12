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
	speed(100.f), maxSpeed(100),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	timer(0.f), moveTime(2.f), stopTime(1.f), stunTime(3.f), dashCoolTime(5.f), hitTime(1.f), hitTimer(0.f),
	hp(500), maxHp(500), barScaleX(60.f), look(1.f, 0.f),
	dashRange(1500.f), dashSpeed(800.f), range(500.f), activeDashRange(600.f), dashDamage(100), damage(10),
	isHit(false), isInSight(true), isStart(false), isDash(false), isStun(false),
	dashAttack(0)
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
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigStun"));
		animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("BossBigStunLeft"));
		dashHitbox = new HitBox();
		//dashHitbox->SetActive(false);
		dashHitbox->SetFillColor(Color::Red);
		dashHitbox->SetPos({ GetPos().x - 100.f,GetPos().y - 60.f });
		//dashHitbox.set
		dashHitbox->SetHitbox({ 0.f,0.f,200.f,100.f });
		break;
	case Boss::Type::Kiba:
		break;
	default:
		break;
	}

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
	range = dashRange;
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
	case Boss::States::Dash:
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
	CheckIsInSight();
	//ready
	//if got shot from range will be problem
	//need change
	if (isStart == false && (Utils::Distance(player->GetPos(), GetPos()) < 1000.f))
	{
		isStart = true;
	}

	//start fight
	if (isStart)
	{
		lookDir = Utils::Normalize(player->GetPos() - GetPos());
		direction.x = (player->GetPos().x > GetPos().x) ? 1.f : -1.f;
		lastDirection = direction;
		
		//enemy dead
		if (hp <= 0)
		{
			SetState(States::Dead);
		}

		//enemy attack
		if (!isStun)
		{
			if (currState == States::Idle)
			{
				timer -= dt;
				if (timer <= 0.f)
				{
					if (dashTimer <= 0.f && (Utils::Distance(player->GetPos(), GetPos()) >= activeDashRange))
					{
						SetDashPos();
					}
					else
					{
						AttackPattern(dt);
						
					}

				}

			}
			if (isDash && currState == States::Dash)
			{
				Dash(dt);	
			}
			else
			{
				dashTimer -= dt;	
			}
			//move
			if (currState == States::Move)
			{
				Move(dt);
				timer -= dt;
				if (timer <= 0.f)
				{
					SetState(States::Idle);
					movePos.clear();
					timer = stopTime;
				}
			}
			ContactDamage();
		}
		else
		{
			timer -= dt;
			if (timer <= 0.f)
			{
				SetState(States::Idle);
				isStun = false;
			}
		}
		
		
		//contact hit timer
		hitTimer -= dt;

		//hp bar
		SetHpBar();

		//animation
		animator.Update(dt);

		//position
		bottomPos = bottom->GetHitBottomPos();
		dashHitbox->SetPos({ GetPos().x - 100.f,GetPos().y - 60.f });
		//gun
		//gun->Update(dt);

		//dev
		if (InputMgr::GetKeyDown(Keyboard::Z))
		{
			SetDashPos();
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
	dashHitbox->Draw(window);
	if (GetActive() && isInSight)
	{
		HitBoxObject::Draw(window);
		window.draw(healthBar);
		SetColor(Color::White);
	}
	VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		lines[0].position = { GetPos().x,GetPos().y + 20.f };
		lines[1].position = { player->GetPos().x,player->GetPos().y + 20.f };
		window.draw(lines);
	}

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
	hp -= num;
	if (hp <= 0)
	{
		hp = 0;
	}


	/*string ments[3] = { "Ouch..!", "Oh No!" , "Fuxx" };
	Vector2f randPos = { Utils::RandomRange(-20,20) + GetPos().x,
		Utils::RandomRange(-50,-40) + GetPos().y + 30.f };
	Ment* ment = new Ment();
	ment->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"), 30, Color::White, ments[Utils::RandomRange(0, 3)]);
	ment->SetAlways(false);
	ment->SetTimer(1.f);
	ment->SetOrigin(Origins::BC);
	ment->SetPos(randPos);
	ment->SetActive(true);
	SCENE_MGR->GetCurrScene()->AddGameObject(ment, LayerType::Object, 1);*/
}

void Boss::SetHpBar()
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
	//attack motion
	lookDir = Utils::Normalize(player->GetPos() - GetPos());
	//gun->SetLookDir(lookDir);
	//gun->Fire(GetPos(), false);
	timer = moveTime;
	playerPos = player->GetPos();
	movePos.clear();
	FindGrid();
	astar->AstarSearch(*isGreedObject, startPos, destPos);
	movePos = astar->GetCoordinate();
	SetState(States::Move);
}

void Boss::Move(float dt)
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

void Boss::Dash(float dt)
{
	prevPosition = GetPos();
	if (range <= 0.f)
	{
		speed = maxSpeed;
		movePos.clear();
		isDash = false;
		SetState(States::Idle);
		range = dashRange;
		dashTimer = dashCoolTime;
		//dashHitbox->SetActive(false);
		return;
		//cout << "dash fin" << endl;
	}
	else
	{
		speed = dashSpeed;
	}
	Translate(dashDir * dt * speed);
	range -= Utils::Magnitude(dashDir * dt * speed);
	Collision();
	DashCollision();
	
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
				{
					SetBossPos();
					if (currState == States::Dash)
					{
						Stun();
					}
				}
			}
		}
	}
}

void Boss::DashCollision()
{
	for (auto& pHit : player->GetHitBoxs())
	{
		if (dashHitbox->GetActive() && Utils::OBB(dashHitbox->GetHitbox(), pHit->GetHitbox()))
		{
			if (dashAttack <= 0)
			{
				player->SetHp(dashDamage);
				player->SetStun(true);
				dashAttack++;
			}
			//dashHitbox->SetActive(false);
			break;
		}
	}
}

void Boss::ContactDamage()
{
	for (auto& pHit : player->GetHitBoxs())
	{
		if (dashHitbox->GetActive() && Utils::OBB(dashHitbox->GetHitbox(), pHit->GetHitbox()))
		{
			if (hitTimer <= 0)
			{
				player->SetHp(damage);
				hitTimer = hitTime;
				cout << "damage" << endl;
			}
			break;
		}
	}
}

void Boss::SetDashPos()
{
	movePos.clear();
	dashDir = Utils::Normalize(player->GetPos() - GetPos());
	isDash = true;
	dashAttack = 0;
	SetState(States::Dash);
}

void Boss::Stun()
{
	isStun = true;
	timer = stunTime;
	speed = maxSpeed;
	movePos.clear();
	isDash = false;
	range = dashRange;
	dashTimer = dashCoolTime;
	SetState(States::Idle);
	//dashHitbox->SetActive(false);
	animator.Play((direction.x > 0.f) ? "BossBigStun" : "BossBigStunLeft");
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
