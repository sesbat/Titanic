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


	userInfo.hp = 100.f;
	userInfo.energyGuage = 255.f;
	userInfo.hungerGuage = 255.f;
	userInfo.thirstGuage = 255.f;

	ifstream info_craft("config/data/craft/CraftTable1.json");
	json info_craft_d = json::parse(info_craft);
	craftItemInfo = info_craft_d;
	info.close();

	ifstream info_user("config/data/gameData/userInfo.json");
	json info_user_d = json::parse(info_user);
	userInfo = info_user_d;
	info_user.close();

	ifstream info_inven("config/data/gameData/invenInfo.json");
	json info_inven_d = json::parse(info_inven);
	invenAllItems = info_inven_d;
	info_inven.close();

	ifstream info_inven_use("config/data/gameData/invenUseInfo.json");
	json info_inven_use_d = json::parse(info_inven_use);
	useItemInfo = info_inven_use_d;
	info_inven_use.close();

	ifstream info_shotGun("config/data/gunData/shotgunstat.json");
	json info_shotGun_d = json::parse(info_shotGun);
	shotgunInfo = info_shotGun_d;
	info_shotGun.close();

	ifstream info_rifle("config/data/gunData/riflestat.json");
	json info_rifle_d = json::parse(info_rifle);
	rifleInfo = info_rifle_d;
	info_rifle.close();

	ifstream info_sniper("config/data/gunData/sniperstat.json");
	json info_sniper_d = json::parse(info_sniper);
	sniperInfo = info_sniper_d;
	info_sniper.close();

	ifstream info_user_stat("config/data/gameData/userStat.json");
	json info_user_stat_d = json::parse(info_user_stat);
	userStat = info_user_stat_d;
	info_user_stat.close();

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

void FileManager::SaveUseItemInfo(vector<InvneUseInfo> datas)
{
	useItemInfo = datas;

	json data = useItemInfo;
	ofstream ofs("config/data/gameData/invenUseInfo.json");
	ofs << data;
	ofs.close();
}

