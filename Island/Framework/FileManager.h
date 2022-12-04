#pragma once
#include "../3rd/Singleton.h"
#include "FileData.h"

class FileManager : public Singleton<FileManager>
{
private:
	map<string, vector<ns::RectangleInfo>> hitBoxData; //find by path
	map<string, vector<ObjectData>> mapInfo;
	map<string, vector<EditorObjs>> editorObjs;
	map<string, ItemInfo> itemInfos;
	map<string, CraftingInfo> craftItemInfo;
	map<string, CraftingInfo> itemInfo;

	UserInfo userInfo;
	std::vector<InvenInfo> invenAllItems;
	std::vector<InvneUseInfo> useItemInfo;

	//cookieInfo[Jungle][Jump]
public:
	FileManager();
	~FileManager();
		
	void LoadAll();
	const vector<ns::RectangleInfo>& GetHitBox(string name);

	const vector<ObjectData>& GetMap(string name);
	const map<string, vector<ObjectData>>& GetAllMap() { return mapInfo; };

	void SaveMap(vector<ObjectData> newData, string name);
	const map<string, vector<EditorObjs>>& GetEditorObjs() { return editorObjs; }

	const map<string, ItemInfo>& GetAllItems() { return itemInfos; }
	const ItemInfo& GetItemInfo(string name) { return itemInfos[name]; }

	const map<string, CraftingInfo>& GetAllCraft() { return craftItemInfo; }
	const CraftingInfo& GetCraftItemInfo(string name) { return craftItemInfo[name]; }

	void SaveUserInfo(UserInfo datas);
	void SaveInvenInfo(vector<InvenInfo> datas);
	void SaveUseItemInfo(vector<InvneUseInfo> datas);

	const UserInfo& GetUserInfo() const { return userInfo; }
	const vector<InvenInfo>& GetInvenInfo() const { return invenAllItems; }
	const vector<InvneUseInfo>& GetUseItemInfo() const { return useItemInfo; }
};

#define FILE_MGR (FileManager::GetInstance())