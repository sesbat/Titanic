#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

FileManager::FileManager()
{
	//{
	//	CookieHitBox hit;
	//	ns::RectangleInfo bottom{ { 40, 10 }, { 180, 354  } };
	//	ns::RectangleInfo body{ { 45.f,52.f }, { 176,300 } };
	//	ns::CircleInfo cir{ 38,sf::Vector2f{ 186,234} };
	//	hit.type = "Run";
	//	hit.bottom = bottom;
	//	hit.hitBox.circles.push_back(cir);
	//	hit.hitBox.rectangls.push_back(body);
	//	hit.hitBox.rectangls.push_back(bottom);
	//	cookieInfo["Jungle"]["Run"] = hit;
	//	bottom = { { 40, 10 }, { 180, 354  } };
	//	body = { { 45.f,52.f }, { 176,300 } };
	//	CookieHitBox hit1;
	//	hit1.type = "Slide";
	//	hit1.bottom = bottom;
	//	hit1.hitBox.circles.push_back(cir);
	//	hit1.hitBox.rectangls.push_back(body);
	//	hit1.hitBox.rectangls.push_back(bottom);
	//	cookieInfo["Jungle"]["Slide"] = hit1;
	//	ofstream cookie1("config/data/cookieInfo.json");
	//	json data = cookieInfo;
	//	cookie1 << data;
	//	
	//}
	//{
	//	map<string, vector<ObjectData>> stage;
	//	vector<ObjectData> objData;
	//	ObjectData d;
	//	d.type = "TREE";
	//	d.path = "graphics/editor/tree1.png";
	//	d.uiPath = "graphics/editor/drawTree1.png";
	//	d.position = sf::Vector2f(100.f,100.f);
	//	d.bottom = {};
	//	d.hitBox.push_back({});
	//	d.hitBox.push_back({});
	//	d.item.push_back({});
	//	d.item.push_back({});
	//	d.randomItem.push_back({});
	//	d.randomItem.push_back({});
	//	
	//	objData.push_back(d);
	//	stage["Tutorial"] = objData;
	//	ofstream cookie1("config/data/map.json");
	//	json data = stage;
	//	cookie1 << data;
	//}
	//{
	//	map<string, vector<EditorObjs>> objData;
	//	vector<EditorObjs> paths;
	//	paths.push_back({ "graphics/editor/tree1.png" ,"graphics/editor/drawTree1.png" });
	//	paths.push_back({ "graphics/editor/tree2.png" ,"graphics/editor/drawTree2.png"});
	//	objData["TREE"] = paths;
	//	ofstream allObj("config/data/allObjs.json");
	//	json data = objData;
	//	allObj << data;
	//}

	//{
	//	CraftingInfo testItem;
	//	testItem.path = "graphics/items/hammer.png";

	//	RequiredItem useItem;
	//	useItem.path = "graphics/material/stone.png";
	//	useItem.cnt = 1;

	//	RequiredItem useItem2;
	//	useItem2.path = "graphics/material/wood.png";
	//	useItem2.cnt = 1;

	//	testItem.useItem.push_back(useItem);
	//	testItem.useItem.push_back(useItem2);

	//	testItem;

	//	itemInfo[testItem.path] = testItem;

	//	json data = itemInfo;
	//	ofstream ofs("config/data/CraftTable.json");
	//	ofs << data;
	//	ofs.close();
	//}

	ifstream info("config/data/CraftTable.json");
	json info_d = json::parse(info);
	craftItemInfo = info_d;
	info.close();

}
FileManager::~FileManager()
{
}

void FileManager::LoadAll()
{
	for (const auto& file : std::experimental::filesystem::directory_iterator("config/data/map/"))
	{
		auto path = file.path().string();
		auto name = file.path().string();
		name = name.substr(16, name.size() - 17 + 1);
		name = name.substr(0, name.size() - 5);
		cout << name << endl;
		ifstream allMap(path);
		json allMap_d = json::parse(allMap);
		mapInfo[name] = allMap_d;
		allMap.close();
	}

	ifstream ao("config/data/allObjs.json");
	json ao_d = json::parse(ao);
	editorObjs = ao_d;
	ao.close();

	ifstream info("config/data/hitBoxs.json");
	json info_d = json::parse(info);
	hitBoxData = info_d;
	info.close();

	ifstream item_info("config/data/allItems.json");
	json item_info_d = json::parse(item_info);
	itemInfos = item_info_d;
	item_info.close();


	//ifstream hitbox("config/data/hitBox.json");
	//json hit_d = json::parse(hitbox);
	//hitBoxMap = hit_d;
	//hitbox.close();

}

const vector<ObjectData>& FileManager::GetMap(string name)
{
    return mapInfo[name];
}

const vector<ns::RectangleInfo>& FileManager::GetHitBox(string name)
{
	return hitBoxData[name];
}

void FileManager::SaveMap(vector<ObjectData> newData, string name)
{
	mapInfo[name] = newData;

	json data = newData;
	ofstream ofs("config/data/map/" + name + ".json");
	ofs << data;
	ofs.close();
}

