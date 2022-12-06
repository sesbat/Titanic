#pragma once
#include "Button.h"

class MoveObject : public Button
{
private:
	MoveObject(const MoveObject& ref) : Button(nullptr) {}
	MoveObject& operator=(const MoveObject& ref) {}
	MoveObject(MoveObject& ref) : Button(nullptr) {}
	MoveObject& operator=(MoveObject& ref) {}
	Vector2f initPos;

public:
	MoveObject(UiMgr* scene);
	virtual ~MoveObject();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Init();
	virtual void Reset();
};

