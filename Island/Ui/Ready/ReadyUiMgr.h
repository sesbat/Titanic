#pragma once
#include "../UiMgr.h"

class ReadyButton;
class Scene;
class ReadyUiMgr : public UiMgr
{
private:
	ReadyButton* episode;
	ReadyButton* editor;

public:
	ReadyUiMgr(Scene* scene);
	virtual void Update(float dt) override;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	void ClickEpisodeBtn();

	void ClickEditortBtn();

};

