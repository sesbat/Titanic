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
}

void ReadyButton::Draw(RenderWindow& window)
{
	sprite->Draw(window);
}
