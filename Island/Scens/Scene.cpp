#include "Scene.h"
#include <iostream>
#include "../GameObject/Object.h"
#include "../Framework/Framework.h"
#include "../Ui/Menu/MenuUiMgr.h"

Scene::Scene(Scenes type) :type(type), uiMgr(nullptr)
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

	if(uiMgr != nullptr)
		uiMgr->Draw(window);
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
