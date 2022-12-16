#pragma once
#include "../UiMgr.h"

class Fade;
class Button;
class Scene;
class MoveObject;
class DblClkObject;
class MenuUiMgr : public UiMgr
{
private:
	Button* startBtn0;
	Button* startBtn1;
	Button* startBtn2;
	Button* startBtn3;

	MoveObject* move;
public:
	MenuUiMgr(Scene* scene);
	virtual void Update(float dt) override;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
		
	void ClickStartBtn();
};

