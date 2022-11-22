#pragma once
#include "Scene.h"
#include "../../GameObject/Animation/Animator.h"
class SpriteObject;
class Button;
class TextObject;
class Menu : public Scene
{
protected:
	SpriteObject* backGround;
	SpriteObject* button;

	Vector2f buttonPos[3];
	TextObject* start;
	TextObject* settings;
	TextObject* exit;
public:
	Menu();
	~Menu();                                                                                                                                                              
	virtual void Init() override;
	virtual void Release() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;

	// Scene��(��) ���� ��ӵ�
	virtual void Reset() override;
};

