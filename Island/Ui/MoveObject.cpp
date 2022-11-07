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
	
	if(IsDragRight())
		SetPos(InputMgr::GetMousePos());
}

void MoveObject::Init()
{
	Button::Init();
}

void MoveObject::Reset()
{
	Button::Reset();
}
