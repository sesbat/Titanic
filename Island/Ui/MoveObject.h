#pragma once
#include "Button.h"

class MoveObject : public Button
{
private:
	Vector2f initPos;

public:
	MoveObject(UiMgr* scene);
	virtual ~MoveObject();
	virtual void Update(float dt);
	virtual void Init();
	virtual void Reset();
};

