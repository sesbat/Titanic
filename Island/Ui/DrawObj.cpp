#include "DrawObj.h"
#include "../../Framework/InputMgr.h"
#include "../../GameObject/SpriteObject.h"
#include "../../Scens/Scene.h"
#include "../../Scens/SceneManager.h"

DrawObj::DrawObj(UiMgr* uimgr)
	:Button(uimgr), isMove(true)
{
	SetMove(true);
}

DrawObj::~DrawObj()
{
}

void DrawObj::Update(float dt)
{
	Button::Update(dt);

	if (isMove)
	{
		auto mousePos = InputMgr::GetMousePos();
		mousePos = SCENE_MGR->GetCurrScene()->ScreenToUiPosition((Vector2i)mousePos);
		SetPos(mousePos);
	}

}

void DrawObj::SetMove(bool move)
{
	isMove = move;
}
