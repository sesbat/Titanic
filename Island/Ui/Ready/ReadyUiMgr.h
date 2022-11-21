#pragma once
#include "../UiMgr.h"
#include <vector>

class ReadyButton;
class Scene;
class Player;
class NPC;
class Button;
class ReadyUiMgr : public UiMgr
{
private:
	Player* player;
	NPC* npc;

	UiMgr* uiMgr;

	vector<Button*> maps;
	Button* mapsBK;
public:
	ReadyUiMgr(Scene* scene);
	virtual void Init();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void ClickEpisodeBtn();

	void ClickEditortBtn();

};

