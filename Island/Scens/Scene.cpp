#include "Scene.h"
#include <iostream>
#include "../GameObject/Object.h"
#include "../Framework/Framework.h"
#include "../Ui/Menu/MenuUiMgr.h"
#include "../GameObject/HitBoxObject.h"
#include "../GameObject/HitBox.h"
#include "../Scens/GameScene.h"
#include "../Scens/Ready.h"
#include "../GameObject/Player.h"

Scene::Scene(Scenes type) :type(type), uiMgr(nullptr), isGameScene(false)
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
	moves.clear();
	drawObjs.clear();
	alphaObj.clear();
	another.clear();

	if (uiMgr != nullptr)
	{
		uiMgr->Release();
		uiMgr = nullptr;
	}
	uiMgr = nullptr;
	for (auto& layer : objList)
	{
		for (auto& obj_pair : layer.second)
		{
			for (auto& obj : obj_pair.second)
			{
				if (obj != nullptr)
				{
					delete obj;
				}
				obj = nullptr;
			}
			obj_pair.second.clear();
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
	if (!isGameScene)
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
		if (uiMgr != nullptr)
			uiMgr->Update(dt);

		return;
	}

	for (auto& obj : drawObjs)
	{
		obj->Update(dt);

	}
	for (auto& obj : objList[LayerType::Object][1])
	{
		obj->Update(dt);
	}

	for (auto& del : deleteContainer)
	{
		auto it = find(objList[LayerType::Object][0].begin(), objList[LayerType::Object][0].end(), del);
		if (it != objList[LayerType::Object][0].end())
		{
			auto ptr = *it;
			objList[LayerType::Object][0].erase(it);
			delete ptr;
			continue;
		}

		it = find(objList[LayerType::Object][1].begin(), objList[LayerType::Object][1].end(), del);
		if (it != objList[LayerType::Object][1].end())
		{
			auto ptr = *it;
			objList[LayerType::Object][1].erase(it);
			delete ptr;
			continue;
		}
	}
	if (deleteContainer.size() > 0)
	{
		LayerSort();
	}
	deleteContainer.clear();

	if(uiMgr != nullptr)
		uiMgr->Update(dt);
}

void Scene::Draw(RenderWindow& window)
{
	window.setView(worldView);

	if (!isGameScene)
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
		int i = 0;
		for (auto& obj : objList[LayerType::Tile])
		{
			for (auto& o : obj.second)
			{
				if (((SpriteObject*)o)->IsInView())
					o->Draw(window);
			}
		}
		for (auto& obj : another)
		{
			obj->Draw(window);
		}
		for (auto& obj : drawObjs)
		{
			obj->Draw(window);
		}
		for (auto& obj : objList[LayerType::Object])
		{
			if (obj.first == 0)
				continue;
			for (auto& o : obj.second)
			{
				o->Draw(window);
			}
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
	another.clear();
	//radPos.clear();
	radObj.clear();
	HitBoxObject* player = nullptr;

	for (auto& obj : alphaObj)
	{
		((HitBoxObject*)(obj))->SetHitPlayer(false);
	}

	alphaObj.clear();

	for (auto& obj : objList[LayerType::Object][0])
	{
		if (!(((SpriteObject*)obj)->IsInView()))
		{
			continue;
		}
		if (obj->GetName() == "TREE" || obj->GetName() == "BUSH" || obj->GetName() == "STONE" ||
			obj->GetName() == "BLOCK" || obj->GetName() == "BOX" || obj->GetName() == "SupplyBox" ||
			obj->GetName() == "BOX-ENEMY" || obj->GetName() == "RADIATION"|| obj->GetName() == "INVISIBLE"|| obj->GetName() == "RADTILE")
		{
			if (obj->GetName() == "TREE" || obj->GetName() == "BUSH")
				alphaObj.push_back((HitBoxObject*)obj);
			if (obj->GetName() == "RADIATION" || obj->GetName() == "RADTILE")
			{
				//radPos.push_back(obj->GetPos());
				radObj.push_back(obj);
			}

			if(obj->GetActive())
				drawObjs.push_back(obj);

		}
		else if (obj->GetName() == "ANOTHER")
		{
			another.push_back(obj);
		}
		else if (obj->GetName() == "ENEMY" || obj->GetName() == "BOSS" || obj->GetName() == "PLAYER" || obj->GetName() == "NPC")
		{
			if (obj->GetName() == "PLAYER")
				player = ((HitBoxObject*)obj);

			if (obj->GetActive())
				moves.push_back(obj);
		}
	}

	bool playerIsHide = false;
	if (player != nullptr)
	{
		for (auto& obj : alphaObj)
		{
			if (Utils::OBB(obj->GetHitBoxs(), player->GetBottom()))
			{
				obj->SetHitPlayer(Utils::OBB(obj->GetHitBoxs(), player->GetBottom()));
				if(obj->GetName() == "BUSH")
					playerIsHide = true;
			}
		}
		((Player*)player)->SetHide(playerIsHide);
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

void Scene::AddDeleteObject(int layer_idx, Object* obj)
{
	auto it = find(objList[LayerType::Object][layer_idx].begin(), objList[LayerType::Object][layer_idx].end(), obj);
	if (it != objList[LayerType::Object][layer_idx].end())
	{
		deleteContainer.push_back(*it);
		(*it)->SetActive(false);
	}
}

