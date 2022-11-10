#include "Player.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/SoundManager.h"
#include "../Scens/SceneManager.h"
#include "VertexArrayObj.h"
#include "Bullet.h"
#include "Object.h"
#include "HitBox.h"
#include "../Scens/SceneDev2.h"
#include <iostream>
#include "Gun.h"


Player::Player()
	: currState(States::None), speed(500.f),
	look(1.f, 0.f), prevLook(1.f, 0.f),
	direction(1.f, 0.f), lastDirection(1.f, 0.f),
	hp(10), maxHp(10)
{
}

Player::~Player()
{
}

void Player::Init()
{
	HitBoxObject::Init();
	hp = maxHp;

	shotgun = new Gun(GunType::Shotgun);
	shotgun->SetPlayer(this);
	shotgun->Init();

	/*rifle = new Gun(GunType::Rifle);
	rifle->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
	rifle->SetPos({ GetPos().x,GetPos().y + 10.f });
	rifle->SetOrigin(Origins::MC);

	sniper = new Gun(GunType::Sniper);
	sniper->SetTexture(*RESOURCES_MGR->GetTexture("graphics/shotgun.png"));
	sniper->SetPos({ GetPos().x,GetPos().y + 10.f });
	sniper->SetOrigin(Origins::MC);*/
	
	animator.SetTarget(&sprite);

	//health bar
	healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ 6.f * maxHp, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	Utils::SetOrigin(healthBar, Origins::MC);

	//animation
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerIdle"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerMove"));
	
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerIdleLeft"));
	animator.AddClip(*ResourceManager::GetInstance()->GetAnimationClip("PlayerMoveLeft"));
	
	scene = SCENE_MGR->GetCurrScene();
	
	SetState(States::Idle);
}

void Player::SetState(States newState)
{

	currState = newState;
	switch (currState)
	{
	case Player::States::Idle:
		animator.Play((lookDir.x > 0.f) ? "PlayerIdle" : "PlayerIdleLeft");
		break;
	case Player::States::Move:
		animator.Play((lookDir.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection = direction;
		break;
	}
}

void Player::Update(float dt)
{
	HitBoxObject::Update(dt);
	Vector2i mousePos = (Vector2i)InputMgr::GetMousePos();
	Vector2f mouseWorldPos = scene->ScreenToWorld(mousePos);

	look = mouseWorldPos;
	lookDir = Utils::Normalize(mouseWorldPos - GetPos());
	direction.x = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.y = InputMgr::GetAxisRaw(Axis::Vertical);

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	}

	//����
	/*if ( hp <= 0 )
	{
		SetState(States::Dead);
	}*/

	//����
	velocity = direction * speed;

	//����
	if ( Utils::Magnitude(direction) == 0.f )
	{
		velocity = { 0.f, 0.f };
	}
	if ( direction.x == 0.f )
	{
		velocity.x = 0.f;
	}
	if ( direction.y == 0.f )
	{
		velocity.y = 0.f;
	}

	prevPosition = GetPos();
	Translate(velocity * dt);
	

	//attack
	//timer += dt;
	
	//positions
	for (auto& hit : hitboxs)
	{
		hit->SetPos(GetPos());
	}

	//hp bar
	SetHpBar();

	//animation
	animator.Update(dt);
	
	//wall bound
	auto obj = scene->GetObjList();
	//for (auto& objTile : obj[LayerType::Tile][0])
	//{
	//	auto hit = ((HitBoxObject*)objTile)->GetBottom();
	//	//if (hit == nullptr /*|| !((SpriteObject*)objTile)->IsInView()*/)
	//	//	continue;
	//	if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
	//	{
	//		std::cout << "wall" << std::endl;
	//		SetPlayerPos();
	//	}

	//}
	for (auto& objects : obj[LayerType::Object][0])
	{
		auto hit = ((HitBoxObject*)objects)->GetBottom();
		if (hit == nullptr || !((SpriteObject*)objects)->IsInView())
			continue;
		if (objects->GetName() == "TREE" ||
			objects->GetName() == "STONE" ||
			objects->GetName() == "ENEMY")
		{
			if (Utils::OBB(hit->GetHitbox(), bottom->GetHitbox()))
			{
				SetPlayerPos();
				break;
			}
		}
	}
	
	if (!EqualFloat(direction.x, 0.f))
	{
		lastDirection = direction;
	}
	prevLook = lookDir;


	shotgun->Update(dt);

}

void Player::Draw(RenderWindow& window)
{
	HitBoxObject::Draw(window);
	window.draw(healthBar);
	if (isHitBox)
	{
		for (auto& hit : hitboxs)
		{
			hit->Draw(window);
		}
	}
	shotgun->Draw(window);
}

void Player::Dash(float dt)
{
	Translate(direction * speed * dt);
}

void Player::UpdateIdle(float dt)
{
	//attackHitbox->SetActive(false);
	if ( !EqualFloat(direction.x, 0.f) )
	{
		SetState(States::Move);
	}
	else if ( !EqualFloat(direction.y, 0.f) )
	{
		SetState(States::Move);
	}
	if ((lookDir.x > 0 && prevLook.x < 0) ||
		(lookDir.x < 0 && prevLook.x > 0))
	{
		SetState(States::Idle);
	}
}

void Player::UpdateMove(float dt)
{
	if (EqualFloat(direction.x, 0.f) && EqualFloat(direction.y, 0.f))
	{
		SetState(States::Idle);
		return;
	}

	if ( (lookDir.x > 0 && prevLook.x < 0) || 
		(lookDir.x < 0 && prevLook.x > 0))
	{
		//animator.Play((lookDir.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		SetState(States::Move);
	}

}

bool Player::EqualFloat(float a, float b)
{
	return fabs(a - b) < numeric_limits<float>::epsilon();
}

void Player::SetHp(int num)
{
	if ( hp > 0 )
	{
		hp -= num;
	}
	else
	{
		hp = 0;
	}
}

void Player::SetHpBar()
{
	healthBar.setPosition({ GetPos().x, GetPos().y - 35.f });
	healthBar.setSize({ 6.f * hp, 15.f });
	if ( hp > 5 )
	{
		healthBar.setFillColor(Color::Green);
	}
	else if ( hp <= 5 && hp > 2 )
	{
		healthBar.setFillColor(Color::Yellow);
	}
	else if ( hp <= 2 )
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

//void Player::OnPickupItem(Item* item)
//{
//	switch ( item->GetType() )
//	{
//	case Item::Types::Potion:
//		cout << "potion" << endl;
//		hp += item->GetValue();
//		if ( hp >= maxHp )
//			hp = maxHp;
//		break;
//	case Item::Types::Coin:
//		cout << "coin" << endl;
//		//exp += item->GetValue();
//		break;
//	case Item::Types::Key:
//		cout << "key" << endl;
//		item->SetIsKey();
//		isKey = true;
//		break;
//	}
//}

void Player::SetPlayerPos()
{
	SetPos(prevPosition);
	for (auto& hit : hitboxs)
	{
		hit->SetPos(prevPosition);
	}
	bottom->SetPos(prevPosition);
	healthBar.setPosition({ prevPosition.x, prevPosition.y - 35.f });
}

Vector2f Player::SetLookDir()
{
	Vector2f dir;
	dir.x = look.x - GetPos().x;
	dir.y = 0.f;
	return dir;
}

void Player::SetFlipX(bool flip)
{
	SpriteObject::SetFlipX(flip);
}
