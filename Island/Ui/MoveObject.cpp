#include "MoveObject.h"
#include "../../Framework/InputMgr.h"
#include <iostream>
#include "../Scens/SceneManager.h"
MoveObject::MoveObject(UiMgr* scene)
	:Button(scene)
{
	SetMove(true);
}

MoveObject::~MoveObject()
{
	Release();
}

void MoveObject::Release()
{
	Button::Release();
}

void MoveObject::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	
	if (IsDown())
	{
	}
	if (IsDrag())
	{
		auto MousePos = InputMgr::GetMousePos();
		MousePos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)MousePos);
		SetPos(MousePos + Vector2f{ -30.f, -30.f });
	}
}

void MoveObject::Init()
{
	Button::Init();
}

void MoveObject::Reset()
{
	Button::Reset();
}
