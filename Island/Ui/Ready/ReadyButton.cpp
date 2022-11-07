#include "ReadyButton.h"
#include <iostream>
#include "../../GameObject/SpriteObject.h"
#include "ReadyUiMgr.h"

ReadyButton::ReadyButton(ReadyUiMgr* mgr)
	:mgr(mgr), Button(mgr)
{
	sprite = new SpriteObject();
}

void ReadyButton::SetScene(Scenes scenes)
{
	this->scene = scenes;
}

void ReadyButton::Update(float dt)
{
	Button::Update(dt);
	//std::cout << (int)btnState << std::endl;
	if(IsDown())
	{
		if (GetName() == "Editor")
			mgr->ClickEditortBtn();
		if (GetName() == "Episode")
			mgr->ClickEpisodeBtn();
	}
	if (IsEnter())
	{
		sprite->SetColor(Color(200, 200, 200, 255));
	}
	if (IsExit())
	{
		sprite->SetColor(Color::White);
	}
}

void ReadyButton::Draw(RenderWindow& window)
{
	sprite->Draw(window);
}
