#pragma once
#include "../UiMgr.h"

class Button;
class Scene;
class MoveObject;
class DblClkObject;
class MenuUiMgr : public UiMgr
{
private:
	DblClkObject* startBtn;
	Button* startBtn2;
	Button* startBtn3;
	Button* startBtn4;
	Button* startBtn5;

	MoveObject* move;
public:
	MenuUiMgr(Scene* scene);
	virtual void Update(float dt) override;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	void ClickStartBtn();
};

