#include "Player.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../Framework/Utils.h"
#include "../Framework/SoundManager.h"
#include "../Scens/SceneManager.h"
#include "VertexArrayObj.h"
#include "Object.h"
#include <iostream>

Player::Player()
	: currState(States::None), speed(500.f), direction(1.f, 0.f), lastDirection(1.f, 0.f), hp(10), maxHp(10)
{
}

Player::~Player()
{
}

void Player::Init()
{
	hp = maxHp;
	
	animator.SetTarget(&sprite);

	//health bar
	/*healthBar.setFillColor(Color::Green);
	healthBar.setOutlineColor(Color::Black);
	healthBar.setOutlineThickness(2.f);
	healthBar.setSize({ 6.f * maxHp, 15.f });
	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
	Utils::SetOrigin(healthBar, Origins::MC);*/

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
	if (currState == newState)
		return;

	currState = newState;
	
	switch (currState)
	{
	case Player::States::Idle:
		animator.Play((look.x > 0.f) ? "PlayerIdle" : "PlayerIdleLeft");
		break;
	case Player::States::Move:
		animator.PlayQueue((look.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection = direction;
		break;
	}
}

void Player::SetBackground(VertexArrayObj* bk)
{
	background = bk;
}

void Player::Update(float dt)
{
	scene = SCENE_MGR->GetCurrScene();

	direction.x = InputMgr::GetAxisRaw(Axis::Horizontal);
	direction.y = InputMgr::GetAxisRaw(Axis::Vertical);

	Vector2i mousePos = (Vector2i)InputMgr::GetMousePos();
	Vector2f mouseWorldPos = scene->ScreenToWorld(mousePos);

	look = Utils::Normalize(mouseWorldPos);

	switch (currState)
	{
	case Player::States::Idle:
		UpdateIdle(dt);
		break;
	case Player::States::Move:
		UpdateMove(dt);
		break;
	}

	//죽음
	/*if ( hp <= 0 )
	{
		SetState(States::Dead);
	}*/

	//가속
	velocity = direction * speed;

	//감속
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
	//playerHitbox->SetPos(GetPos());
	
	//hp bar
	//SetHpBar();

	//animation
	animator.Update(dt);

	//wall bound
	/*for ( const auto& hb: background->GetHitBoxList() )
	{
		if ( Utils::OBB(hb->GetHitbox(), playerHitbox->GetHitbox()) )
		{
			std::cout << "wall" << std::endl;
			SetPlayerPos();
		}
	}*/

	if (!EqualFloat(direction.x, 0.f))
	{
		lastDirection = direction;
	}
}

void Player::Draw(RenderWindow& window)
{
	SpriteObject::Draw(window);
	//window.draw(healthBar);
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
}

void Player::UpdateMove(float dt)
{
	if (EqualFloat(direction.x, 0.f) && EqualFloat(direction.y, 0.f))
	{
		SetState(States::Idle);
		return;
	}

	if ( !EqualFloat(direction.x, lastDirection.x))
	{
		animator.PlayQueue((look.x > 0.f) ? "PlayerMove" : "PlayerMoveLeft");
		lastDirection.x = direction.x;
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

//void Player::SetHpBar()
//{
//	healthBar.setPosition({ GetPos().x, GetPos().y - 15.f });
//	healthBar.setSize({ 6.f * hp, 15.f });
//	if ( hp > 5 )
//	{
//		healthBar.setFillColor(Color::Green);
//	}
//	else if ( hp <= 5 && hp > 2 )
//	{
//		healthBar.setFillColor(Color::Yellow);
//	}
//	else if ( hp <= 2 )
//	{
//		healthBar.setFillColor(Color::Red);
//	}
//	if ( hp <= 0 )
//	{
//		healthBar.setOutlineThickness(0.f);
//	}
//	else
//	{
//		healthBar.setOutlineThickness(2.f);
//	}
//}

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
	//playerHitbox->SetPos(prevPosition);
	//healthBar.setPosition({ prevPosition.x, prevPosition.y - 15.f });
}

Vector2f Player::SetLookDir()
{
	Vector2f dir;
	dir.x = look.x - GetPos().x;
	return dir;
}
