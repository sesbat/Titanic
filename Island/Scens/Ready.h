#pragma once
#include "Scene.h"
#include "../../GameObject/Animation/Animator.h"
class SpriteObject;
class Button;
class Ready : public Scene
{
protected:
	SpriteObject* backGround;
public:
	Ready();
	~Ready();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	// Scene을(를) 통해 상속됨
	virtual void Reset() override;
};

