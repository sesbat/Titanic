#include "LoadWindowBox.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "../Framework/InputMgr.h"

LoadWindowBox::LoadWindowBox(UiMgr* mgr)
	:Button(mgr)
{
}

LoadWindowBox::~LoadWindowBox()
{
	for (auto& del : loadFiles)
	{
		if (del != nullptr)
		{
			delete del;
			del = nullptr;
		}
	}
	loadFiles.clear();
	allMaps.clear();
	idx = 0;
}

void LoadWindowBox::Release()
{
	Button::Release();
	for (auto& del : loadFiles)
	{
		if (del != nullptr)
		{
			delete del;
			del = nullptr;
		}
	}
	loadFiles.clear();
	allMaps.clear();
	idx = 0;
}

void LoadWindowBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/window.png"), true);
	SetClkColor(false);

	allMaps = FILE_MGR->GetAllMap();

	for (auto& maps : allMaps)
	{
		auto* button = new Button(uimgr);

		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			45, Color::White, maps.first, true);
		loadFiles.push_back(button);
	}

	idx = 0;
	float y = 0.f;
	for (int i = 0; i < allMaps.size(); i++)
	{
		if (i < idx || i > idx + 5)
		{
			loadFiles[i]->SetActive(false);
		}
		else
		{
			loadFiles[i]->SetActive(true);
			loadFiles[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
			y += 50.f;
		}
	}
	SetActive(false);
}

void LoadWindowBox::Reset()
{
	allMaps = FILE_MGR->GetAllMap();

	for (auto& del : loadFiles)
	{
		if (del != nullptr)
		{
			delete del;
			del = nullptr;
		}
	}
	loadFiles.clear();

	for (auto& maps : allMaps)
	{
		auto* button = new Button(uimgr);

		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			45, Color::White, maps.first, true);
		loadFiles.push_back(button);
	}
	idx = 0;
	float y = 0.f;
	for (int i = 0; i < allMaps.size(); i++)
	{
		if (i < idx || i > idx + 5)
		{
			loadFiles[i]->SetActive(false);
		}
		else
		{
			loadFiles[i]->SetActive(true);
			loadFiles[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
			y += 50.f;
		}
	}
}

bool LoadWindowBox::IsLoad()
{
	bool nowClick = false;

	for (auto& btn : loadFiles)
	{
		if (btn->IsClick())
		{
			nowClick = true;
			loadPath = btn->GetTextObj()->GetString();
			break;
		}
	}

	return nowClick;
}

void LoadWindowBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	for (auto& btn : loadFiles)
	{
		if (btn->GetActive())
		btn->Draw(window);
	}
}

void LoadWindowBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	for (auto& btn : loadFiles)
	{
		if(btn->GetActive())	
			btn->Update(dt);
	}

	if (IsStay())
	{
		if (InputMgr::GetMouseWheelUp())
		{
			idx = min(idx + 1, (int)allMaps.size());
			float y = 0;
			for (int i = 0; i < allMaps.size(); i++)
			{
				if (i < idx || i > idx + 5)
				{
					loadFiles[i]->SetActive(false);
				}
				else
				{
					loadFiles[i]->SetActive(true);
					loadFiles[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
					y += 50.f;
				}
			}
		}
		if (InputMgr::GetMouseWheelDown())
		{
			idx = max(idx - 1, 0); float y = 0;
			for (int i = 0; i < allMaps.size(); i++)
			{
				if (i < idx || i > idx + 5)
				{
					loadFiles[i]->SetActive(false);
				}
				else
				{
					loadFiles[i]->SetActive(true);
					loadFiles[i]->SetPos(Vector2f{ position.x + 10.f,  position.y + y });
					y += 50.f;
				}
			}
		}
	}
}
