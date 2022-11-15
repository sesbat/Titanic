#include "MoveObject.h"
#include "../../Framework/InputMgr.h"

MoveObject::MoveObject(UiMgr* scene)
	:Button(scene)
{
	SetMove(true);
}

MoveObject::~MoveObject()
{
}

void MoveObject::Update(float dt)
{
	Button::Update(dt);
	
	if (IsDrag())
	{
		SetPos(InputMgr::GetMousePos() + Vector2f{-30.f, -30.f});
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
