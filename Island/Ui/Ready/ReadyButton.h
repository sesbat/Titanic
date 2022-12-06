#pragma once
#include "../Button.h"

class SpriteObject;
class ReadyUiMgr;
enum class Scenes;
class ReadyButton : public Button
{
private:
	ReadyButton(const ReadyButton& ref) : Button(nullptr) {}
	ReadyButton& operator=(const ReadyButton& ref) {}
	ReadyButton(ReadyButton& ref) : Button(nullptr) {}
	ReadyButton& operator=(ReadyButton& ref) {}
	SpriteObject* sprite;
	ReadyUiMgr* mgr;
	Scenes scene;

public:
	ReadyButton(ReadyUiMgr* mgr);
	void SetScene(Scenes scenes);
	SpriteObject* GetSprite() { return sprite; }
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
};

