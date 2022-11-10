#include "Scene.h"
#include <iostream>
#include "../GameObject/Object.h"
#include "../Framework/Framework.h"
#include "../Ui/Menu/MenuUiMgr.h"
#include "../GameObject/HitBoxObject.h"
#include "../GameObject/HitBox.h"

Scene::Scene(Scenes type) :type(type), uiMgr(nullptr), isMap(false)
{
}

Scene::~Scene()
{
	Release();
}

void Scene::Reset()
{
}

void Scene::Release()
{
	if(uiMgr != nullptr)
		((MenuUiMgr*)uiMgr)->Release();
	uiMgr = nullptr;
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;

			for (auto& obj : objs)
			{
				if(obj != nullptr)
					delete obj;
			}
			objs.clear();
		}
		layer.second.clear();
	}
	objList.clear();
}



Vector2f Scene::ScreenToWorld(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos, worldView);
}

Vector2f Scene::ScreenToUiPosition(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos, uiView);
}
void Scene::Update(float dt)
{
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;

			for (auto& obj : objs)
			{
				if (obj->GetActive())
				{
					obj->Update(dt);
				}
			}
		}
	}

	if(uiMgr != nullptr)
		uiMgr->Update(dt);
}

void Scene::Draw(RenderWindow& window)
{
	window.setView(worldView);

	if (!isMap)
	{
		for (auto& layer : objList)
		{
			for (auto& obj_pair : layer.second)
			{
				auto objs = obj_pair.second;
				for (auto& obj : objs)
				{
					if (obj->GetActive())
					{
						obj->Draw(window);
					}
				}
			}
		}
		if (uiMgr != nullptr)
			uiMgr->Draw(window);
	}
	else
	{
		LayerSort();
		for (auto& obj : objList[LayerType::Tile])
		{
			for (auto& o : obj.second)
				o->Draw(window);
		}
		for (auto& obj : drawObjs)
		{
			obj->Draw(window);
		}

		if (uiMgr != nullptr)
			uiMgr->Draw(window);
	}
}

void Scene::AddGameObject(Object* obj, LayerType type, int num)
{
	objList[type][num].push_back(obj);
}

Object* Scene::FindGameObj(string name)
{
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			auto objs = obj_pair.second;
			for (auto& obj : objs)
			{
				if (obj->GetName() == name)
				{
					return obj;
				}
			}
		}
	}
}

bool sorting(Object* p1, Object* p2) 
{
	return ((HitBoxObject*)p1)->GetBottomPos() < ((HitBoxObject*)p2)->GetBottomPos(); 
}
void Scene::LayerSort()
{
	moves.clear();
	drawObjs.clear();

	for (auto& objss : objList[LayerType::Object])
	{
		for (auto& obj : objss.second)
		{
			if (!(((SpriteObject*)obj)->IsInView()))
			{
				continue;
			}
			if (obj->GetName() == "TREE" || obj->GetName() == "STONE")
			{
				drawObjs.push_back(obj);
			}
			else if (obj->GetName() == "ENEMY" || obj->GetName() == "PLAYER")
			{
				moves.push_back(obj);
			}
		}
	}

	sort(moves.begin(), moves.end(), sorting);
	auto dit = drawObjs.begin();

	for (auto mit = moves.begin(); mit != moves.end();)
	{
		if (dit == drawObjs.end())
		{
			while (mit != moves.end())
			{
				drawObjs.push_back(*mit);
				mit++;
			}
			break;
		}
		if (((HitBoxObject*)(*mit))->GetBottomPos() < ((HitBoxObject*)(*dit))->GetBottomPos())
		{

			dit = drawObjs.insert(dit, *mit);
			mit++;
		}
		else
		{
			dit++;
		}
	}

}

