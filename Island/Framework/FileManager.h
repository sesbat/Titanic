#pragma once
#include "../3rd/Singleton.h"
#include "FileData.h"

class FileManager : public Singleton<FileManager>
{
private:
	map<string, vector<ns::RectangleInfo>> hitBoxData; //find by path
	map<string, vector<ObjectData>> mapInfo;
	map<string, vector<EditorObjs>> editorObjs;
	map<string, ItemInfo> itemsInfo;
	map<string, ItemEffect> itemEffect;
	map<string, CraftingInfo> craftItemInfo;
	vector<map<string, CraftingInfo>> craftItemInfos;
	map < string, ShopItemInfo> shopItemInfo;
	vector<map<string, ShopItemInfo>> shopItemInfos;
	map<string, vector<string>> connecntMaps;
	map<string, EnemyInfo> enemysInfo;
	map<string ,vector<InvenInfo>> suppleyBoxInfo;

	UserInfo userInfo;

	map<string, GunStat> gunsInfo;
	//GunStat shotgunInfo;
	//GunStat rifleInfo;
	//GunStat sniperInfo;

	std::vector<InvenInfo> invenAllItems;
	std::vector<InvenInfo> saveAllItems;
	std::vector<InvneUseInfo> useItemInfo;

	UserStat userStat;

public:
	FileManager(const FileManager& ref) {}
	FileManager& operator=(const FileManager& ref) {}
	FileManager();
	~FileManager();

	void LoadAll();
	const vector<ns::RectangleInfo>& GetHitBox(string name);

	const vector<ObjectData>& GetMap(string name);
	const map<string, vector<ObjectData>>& GetAllMap() { return mapInfo; };

	void SaveMap(vector<ObjectData> newData, string name);
	const map<string, vector<EditorObjs>>& GetEditorObjs() { return editorObjs; }

	const map<string, ItemInfo>& GetAllItems() { return itemsInfo; }
	const ItemInfo& GetItemInfo(string name) { return itemsInfo[name]; }

	const vector<map<string, CraftingInfo>>& GetAllCraftTable() { return craftItemInfos; }
	const map<string, CraftingInfo>& GetAllCraft() { return craftItemInfo; }
	const CraftingInfo& GetCraftItemInfo(string name) { return craftItemInfo[name]; }

	const vector<map<string, ShopItemInfo>>& GetAllShopItem() { return shopItemInfos; }

	const map<string, ItemEffect>& GetItemEffect() { return itemEffect; }

	//요구된 아이템 정보
	const RequiredItem& GetRequiredItemInfo(string name, int num) { return craftItemInfo[name].useItem[num]; }

	void SaveUserInfo(UserInfo datas);
	void SaveInvenInfo(vector<InvenInfo> datas);
	void SaveSaveBoxInfo(vector<InvenInfo> datas);
	void SaveUseItemInfo(vector<InvneUseInfo> datas);

	void SaveConnecnt(string name, vector<string> need);

	const map<string, vector<string>>& const GetConnecntInfoAll();

	const GunStat& GetGunInfo(string name)  { return gunsInfo[name]; }
	const UserInfo& GetUserInfo() const { return userInfo; }
	const map<string, GunStat>& GetGunInfoAll() { return gunsInfo; }
	//const GunStat& GetShotGunInfo() const { return shotgunInfo; }
	//const GunStat& GetRifleInfo() const { return rifleInfo; }
	//const GunStat& GetSniperInfo() const { return sniperInfo; }
	const vector<InvenInfo>& GetInvenInfo() const { return invenAllItems; }
	const vector<InvenInfo>& GetSaveBoxInfo() const { return saveAllItems; }
	const vector<InvenInfo>& GetSupplyBoxInfo(string name) { return suppleyBoxInfo[name]; }
	const vector<InvneUseInfo>& GetUseItemInfo() const { return useItemInfo; }
	const UserStat& GetUserStat() const { return userStat; }

	const map<string, EnemyInfo>& GetEnemyInfo() { return enemysInfo; }
};

#define FILE_MGR (FileManager::GetInstance())