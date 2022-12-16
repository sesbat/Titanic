#include "ConnectWindowBox.h"
#include "../Framework/ResourceManager.h"
#include "../GameObject/SpriteObject.h"
#include "../GameObject/TextObject.h"
#include "../Framework/FileManager.h"
#include "../Framework/InputMgr.h"

ConnectWindowBox::ConnectWindowBox(UiMgr* mgr)
	:Button(mgr)
{
}

ConnectWindowBox::~ConnectWindowBox()
{
	Release();
}

void ConnectWindowBox::Release()
{
	Button::Release();
	for (auto& del : allFile)
	{
		if (del != nullptr)
		{
			delete del;
			del = nullptr;
		}
	}
	allFile.clear();
	idx = 0;
}

void ConnectWindowBox::Init()
{
	SetTexture(*RESOURCES_MGR->GetTexture("graphics/editor/window.png"), true);
	SetClkColor(false);

	auto allMaps = FILE_MGR->GetAllMap();

	for (auto& maps : allMaps)
	{
		auto* button = new Button(uimgr);

		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			45, Color::White, maps.first, true);
		allFile.push_back(button);
		fileState[button] = false;
	}

	idx = 0;
	float y = 0.f;
	for (int i = 0; i < allMaps.size(); i++)
	{
		if (i < idx || i > idx + 5)
		{
			allFile[i]->SetActive(false);
		}
		else
		{
			allFile[i]->SetActive(true);
			allFile[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
			y += 50.f;
		}
	}
	SetActive(false);
}

void ConnectWindowBox::Reset(string name)
{
	auto allMaps = FILE_MGR->GetAllMap();
	auto allConnectMap = FILE_MGR->GetConnecntInfoAll();
	vector<string> needsMap = allConnectMap.find(name) != allConnectMap.end() ? allConnectMap[name] : vector<string>(0);

	for (auto& del : allFile)
	{
		if (del != nullptr)
		{
			delete del;
			del = nullptr;
		}
	}
	allFile.clear();
	fileState.clear();

	for (auto& maps : allMaps)
	{
		auto* button = new Button(uimgr);

		button->SetText(*RESOURCES_MGR->GetFont("fonts/6809 chargen.otf"),
			45, Color::White, maps.first, true);
		button->SetClkColor(false);
		allFile.push_back(button);

		fileState[button] = find(needsMap.begin(), needsMap.end(), maps.first) != needsMap.end();
		button->GetTextObj()->SetColor(fileState[button] ? Color::Red : Color::White);
	}
	idx = 0;
	float y = 0.f;
	for (int i = 0; i < allFile.size(); i++)
	{
		if (i < idx || i > idx + 5)
		{
			allFile[i]->SetActive(false);
		}
		else
		{
			allFile[i]->SetActive(true);
			allFile[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
			y += 50.f;
		}
	}
}


void ConnectWindowBox::Draw(RenderWindow& window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	for (auto& btn : allFile)
	{
		if (btn->GetActive())
		btn->Draw(window);
	}
}

void ConnectWindowBox::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);
	for (auto& btn : allFile)
	{
		if(btn->GetActive())	
			btn->Update(dt);
		if (btn->IsClick())
		{
			fileState[btn] = !fileState[btn];
			btn->GetTextObj()->SetColor(fileState[btn] ? Color::Red : Color::White);
			break;
		}
	}

	if (IsStay())
	{
		if (InputMgr::GetMouseWheelUp())
		{
			idx = min(idx + 1, (int)allFile.size());
			float y = 0;
			for (int i = 0; i < allFile.size(); i++)
			{
				if (i < idx || i > idx + 5)
				{
					allFile[i]->SetActive(false);
				}
				else
				{
					allFile[i]->SetActive(true);
					allFile[i]->SetPos(Vector2f{ position.x + 10.f, position.y + y });
					y += 50.f;
				}
			}
		}
		if (InputMgr::GetMouseWheelDown())
		{
			idx = max(idx - 1, 0); float y = 0;
			for (int i = 0; i < allFile.size(); i++)
			{
				if (i < idx || i > idx + 5)
				{
					allFile[i]->SetActive(false);
				}
				else
				{
					allFile[i]->SetActive(true);
					allFile[i]->SetPos(Vector2f{ position.x + 10.f,  position.y + y });
					y += 50.f;
				}
			}
		}
	}
}

vector<string> ConnectWindowBox::GetConnectMaps()
{
	vector<string> data;

	for (auto& d : fileState)
	{
		if(d.second)
			data.push_back(d.first->GetTextObj()->GetString());
	}

	return data;
}
