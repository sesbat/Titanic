#include "Inventory.h"
#include "../Framework/ResourceManager.h"
#include "../Framework/InputMgr.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"

Inventory::Inventory(UiMgr* mgr)
	: Button(mgr)
{
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/inventory.png"), true);
	for (int i = 0; i < height; i++)
	{
		
	}
}

void Inventory::Update(float dt)
{
	Button::Update(dt);
}

void Inventory::Draw(RenderWindow& window)
{
	Button::Draw(window);
}
