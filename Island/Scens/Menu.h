#pragma once
#include "Scene.h"
#include "../../GameObject/Animation/Animator.h"
class SpriteObject;
class Button;
class Menu : public Scene
{
protected:
	SpriteObject* backGround;

	Animator pumpAnimator[3];
	Animator pump2Animator[2];
	Animator bombAnimator[2];
	Animator ghostAnimator[3];

	SpriteObject* pump[3];
	SpriteObject* pump2[2];
	SpriteObject* boomb[2];
	SpriteObject* ghost[3];

	vector<Animator> animes;

	SpriteObject* button;

public:
	Menu();
	~Menu();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	// Scene을(를) 통해 상속됨
	virtual void Reset() override;
};

