#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

FileManager::FileManager()
{

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

		string supplyPath = "config/data/supplyBox/" + name + ".json";
		ifstream ios_supply(supplyPath);
		json supply_d = json::parse(ios_supply);
		suppleyBoxInfo[name] = supply_d;
		ios_supply.close();
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
	itemsInfo = item_info_d;
	item_info.close();


	userInfo.hp = 100.f;
	userInfo.energyGuage = 255.f;
	userInfo.hungerGuage = 255.f;
	userInfo.thirstGuage = 255.f;

	for(int i =1; i<4; i++)
	{
		ifstream info_craft("config/data/craft/CraftTable"+to_string(i)+".json");
		json info_craft_d = json::parse(info_craft);
		craftItemInfo = info_craft_d;
		craftItemInfos.push_back(craftItemInfo);
		info.close();
	}

	ifstream info_user("config/data/gameData/userInfo.json");
	json info_user_d = json::parse(info_user);
	userInfo = info_user_d;
	info_user.close();

	ifstream info_inven("config/data/gameData/invenInfo.json");
	json info_inven_d = json::parse(info_inven);
	invenAllItems = info_inven_d;
	info_inven.close();

	ifstream save_box_inven("config/data/gameData/saveBox.json");
	json info_inven_save_d = json::parse(save_box_inven);
	saveAllItems = info_inven_save_d;
	save_box_inven.close();

	ifstream info_inven_use("config/data/gameData/invenUseInfo.json");
	json info_inven_use_d = json::parse(info_inven_use);
	useItemInfo = info_inven_use_d;
	info_inven_use.close();

	string guninfo_paths[] =
	{
		"config/data/gunData/shotgunstat.json",
		"config/data/gunData/riflestat.json",
		"config/data/gunData/sniperstat.json"
	};

	for (auto path : guninfo_paths)
	{
		ifstream ios_gun(path);
		json gun_info = json::parse(ios_gun);
		ios_gun.close();
		gunsInfo[gun_info["name"]] = gun_info;
	}

	ifstream info_user_stat("config/data/gameData/userStat.json");
	json info_user_stat_d = json::parse(info_user_stat);
	userStat = info_user_stat_d;
	info_user_stat.close();

	ifstream effect_item("config/data/itemEffect.json");
	json effect_item_d = json::parse(effect_item);
	itemEffect = effect_item_d;
	effect_item.close();

	ifstream connect("config/data/mapConnecnt.json");
	json connect_d = json::parse(connect);
	connecntMaps = connect_d;
	connect.close();

	ifstream enemy_info("config/data/Enemy/EnemyInfo.json");
	json enemy_d = json::parse(enemy_info);
	enemysInfo = enemy_d;
	enemy_info.close();
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

void FileManager::SaveUserInfo(UserInfo datas)
{
	userInfo = datas;

	json data = userInfo;
	ofstream ofs("config/data/gameData/userInfo.json");
	ofs << data;
	ofs.close();

}

void FileManager::SaveInvenInfo(vector<InvenInfo> datas)
{
	invenAllItems = datas;

	json data = invenAllItems;
	ofstream ofs("config/data/gameData/invenInfo.json");
	ofs << data;
	ofs.close();
}

void FileManager::SaveSaveBoxInfo(vector<InvenInfo> datas)
{
	saveAllItems = datas;

	json data = saveAllItems;
	ofstream ofs("config/data/gameData/saveBox.json");
	ofs << data;
	ofs.close();
}

void FileManager::SaveUseItemInfo(vector<InvneUseInfo> datas)
{
	useItemInfo = datas;

	json data = useItemInfo;
	ofstream ofs("config/data/gameData/invenUseInfo.json");
	ofs << data;
	ofs.close();
}

void FileManager::SaveConnecnt(string name, vector<string> need)
{
	connecntMaps[name] = need;

	json data = connecntMaps;
	ofstream ofs("config/data/mapConnecnt.json");
	ofs << data;
	ofs.close();
}

const map<string, vector<string>>& FileManager::GetConnecntInfoAll()
{
	return connecntMaps; 
}

