#pragma once
#include "../UiMgr.h"
#include <vector>


class ReadyButton;
class Scene;
class Player;
class NPC;
class Button;
class Inventory;
class Craft;
class ReadyUiMgr : public UiMgr
{
private:
	Player* player;
	NPC* startNpc;
	NPC* craftNpc;

	Inventory* inVen;
	Craft* craft;

	UiMgr* uiMgr;

	vector<Button*> maps;
	Button* mapsBK;

	vector<Button*> categories;
	Button* categoryBK;
public:
	ReadyUiMgr(Scene* scene);
	virtual void Init();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	void ClickEpisodeBtn();

	void ClickEditortBtn();

};

