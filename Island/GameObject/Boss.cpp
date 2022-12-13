#include "Boss.h"
#include "Enemy.h"
#include "Player.h"
#include "HitBox.h"
#include "Object.h"
#include "Gun.h"
#include "Bullet.h"
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

//void OnCreateAcidBullet(Bullet* bullet)
//{
//	GameScene* scene = (GameScene*)SCENE_MGR->GetScene(Scenes::GameScene);
//	bullet->SetTexture(*RESOURCES_MGR->GetTexture("graphics/acid_Thorn.png"));
//	bullet->Init();
//	//bullet->SetBossList(scene->GetBossList());
//	bullet->SetPlayer(scene->GetPlayer());
//}

Boss::Boss()
	: currState(States::None),
	speed(100.f), maxSpeed(100),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	timer(0.f), moveTime(2.f), stopTime(1.f), stunTime(3.f), dashCoolTime(5.f),
	hitTime(1.f), hitTimer(0.f), playerStunTime(2.f),
	hp(1500), maxHp(1500), barScaleX(60.f), look(1.f, 0.f),
	dashSpeed(800.f),
	dashRange(1500.f), range(500.f), activeDashRange(600.f),
	dashDamage(100), damage(10),
	isHit(false), isInSight(true), isStart(false), isDash(false), isStun(false),
	dashAttack(0),
	startRange(1000.f),
	firePattern(0),
	radDamage(5.f),
	fireSpeed(800.f), fireRange(1000.f), shootDelay(1.f), fireCount(15), fireAngle(35.f), rampageCount(15), rampCount(0),
	rampfireRange(1000.f), rampfireSpeed(800.f), rampshootDelay(1.f), rampfireAngle(10.f), rampfireCount(20),
	singleRange(1000.f), singleSpeed(900.f), singleshootDelay(0.5f), singleAngle(0.f), singleCount(1)
{
}

Boss::~Boss()
{
    Release();
}

void Boss::Release()
{
    if (astar != nullptr)
        delete astar;
    astar = nullptr;

	if (gun != nullptr)
		delete gun;
	gun = nullptr;

    HitBoxObject::Release();
}

void Boss::Init(Player* player)
{
	HitBoxObject::Init();
	this->player = player;

	hp = maxHp;
	speed = maxSpeed;

	SetFireVariable();
	
	gun = new Gun(GunType::Shotgun, User::Boss);
	gun->SetBoss(this);
	gun->Init();

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
	if (isStart == false && (Utils::Distance(player->GetPos(), GetPos()) < startRange))
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
			RampagePattern(dt);
			
		}
		//enemy attack
		if (hp > 0)
		{
			if (!isStun)
			{
				if (currState == States::Idle)
				{
					timer -= dt;
					if (timer <= 0.f)
					{
						if (dashTimer <= 0.f && (Utils::Distance(player->GetPos(), GetPos()) <= activeDashRange))
						{
							SetDashPos();
						}
						else if ((Utils::Distance(player->GetPos(), GetPos()) > activeDashRange))
						{
							cout << "pattern " << firePattern << endl;
							if (firePattern >= 2)
							{
								SetFireVariable();
								firePattern = 0;
							}
							else
							{
								SetSingleVariable();
								firePattern++;
							}
							CheckIsInWall();
							gun->SetLookDir(lookDir);
							gun->BossFire(GetPos(), false);
							AttackPattern(dt);
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
		dashHitbox->SetPos({ GetPos().x - 100.f,GetPos().y - 60.f });
		//gun
		gun->Update(dt);

		//dev
		/*if (InputMgr::GetKeyDown(Keyboard::Z))
		{
			CheckIsInWall();
			gun->SetLookDir(lookDir);
			gun->BossFire(GetPos(), false);
		}
		if (InputMgr::GetKeyDown(Keyboard::X))
		{
			SetState(States::Idle);
		}*/
	}
}

void Boss::Draw(RenderWindow& window)
{
	/*if (!enabled || !IsInView())
		return;*/
	//dashHitbox->Draw(window);
	if (GetActive() && isInSight)
	{
		HitBoxObject::Draw(window);
		window.draw(healthBar);
		SetColor(Color::White);
	}
	gun->Draw(window);

	/*VertexArray lines(LineStrip, 2);
	if (isInSight)
	{
		lines[0].position = { GetPos().x,GetPos().y + 20.f };
		lines[1].position = { player->GetPos().x,player->GetPos().y + 20.f };
		window.draw(lines);
	}*/

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
		dashPosition = player->GetPos();
		SetRampageVariable();
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

void Boss::RampagePattern(float dt)
{
	if (rampCount > rampageCount)
	{
		SetState(States::Dead);
		return;
	}
	if (!isDash)
	{
		SetRampagePos();
		
	}
	else if (isDash)
	{
		RampageDash(dt);
	}
	/*else if (!isDash && isStun)
	{
		cout << "count " << rampCount << endl;
		isStun = false;
		SetRampagePos();
		CheckIsInWall();
		gun->SetLookDir(lookDir);
		gun->BossFire(GetPos(), false);
		rampCount++;
	}*/

	
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

void Boss::RampageDash(float dt)
{
	
	if (range <= 0.f)
	{
		movePos.clear();
		isDash = false;
		range = dashRange;
		dashPosition = player->GetPos();
		SetRampagePos();
		return;
	}
	else
	{
		speed = dashSpeed;
	}
	Translate(dashDir * dt * speed);
	range -= Utils::Magnitude(dashDir * dt * speed);
	RampCollision();
	DashCollision();
	prevPosition = GetPos();
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

void Boss::RampCollision()
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
					//cout << "count " << rampCount << endl;
					SetBossPos();
					isStun = true;
					movePos.clear();
					isDash = false;
					range = dashRange;
					dashPosition = dashPosition * -1.f;
					animator.Play((direction.x > 0.f) ? "BossBigMove" : "BossBigMoveLeft");
					//CheckIsInWall();
					gun->SetLookDir(lookDir);
					gun->BossFire(GetPos(), false);
					rampCount++;
					SetRampagePos();
					break;
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
				player->SetStun(true, playerStunTime);
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
				//cout << "damage" << endl;
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

void Boss::SetRampagePos()
{
	movePos.clear();
	dashDir = Utils::Normalize((dashPosition - GetPos()));
	animator.Play((direction.x > 0.f) ? "BossBigMove" : "BossBigMoveLeft");
	isDash = true;
	dashAttack = 0;
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

void Boss::SetFireVariable()
{
	rd = radDamage;
	fr = fireRange;
	fs = fireSpeed;
	sd = shootDelay;
	fa = fireAngle;
	fc = fireCount;
}

void Boss::SetRampageVariable()
{
	fr = rampfireRange;
	fs = rampfireSpeed;
	sd = rampshootDelay;
	fa = rampfireAngle;
	fc = rampfireCount;
}

void Boss::SetSingleVariable()
{
	fr = singleRange;
	fs = singleSpeed;
	sd = singleshootDelay;
	fa = singleAngle;
	fc = singleCount;
}

