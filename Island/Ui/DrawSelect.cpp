#include "DrawSelect.h"
#include "EditorMapUiMgr.h"
#include "../Framework/ResourceManager.h"

DrawSelect::DrawSelect(UiMgr* uimgr)
	:Button(uimgr)
{
}

DrawSelect::~DrawSelect()
{
}

void DrawSelect::Set(string type, string path)
{
	this->type = type;
	this->path = path;

	SetTexture(*RESOURCES_MGR->GetTexture(path),true);
}
void DrawSelect::Update(float dt)
{
	Button::Update(dt);
	if (IsClick())
	{
		cout << "Click" << endl;
		((EditorMapUiMgr*)uimgr)->Select(this);
	}
}

