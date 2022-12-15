#pragma once
#include "UiMgr.h"

class Button;
class Scene;

class TutorialUiMgr : public UiMgr
{
private:
	Button* exit;
public:
	TutorialUiMgr(Scene* scene);
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
};

