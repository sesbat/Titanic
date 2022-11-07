#pragma once
#include "Button.h"

class MoveObject : public Button
{


public:
	MoveObject(UiMgr* scene);
	virtual ~MoveObject();
	virtual void Update(float dt);
	virtual void Init();
	virtual void Reset();
};

