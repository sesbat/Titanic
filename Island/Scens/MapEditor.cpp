#include "MapEditor.h"
#include "../Ui/Button.h"

MapEditor::MapEditor()
	: Scene(Scenes::MapEditor)
{
	Init();
}

void MapEditor::Init()
{
	Reset();
}

void MapEditor::Reset()
{
	for (int i = 0; i < WIDTHCNT; i++)
	{
		for (int j = 0; j < HEIGHTCNT; j++)
		{
			//greeds[i].push_back(new Button(nullptr));
		}
	}
}

void MapEditor::Update(float dt)
{
}

void MapEditor::Draw(float dt)
{
}

MapEditor::~MapEditor()
{
}
