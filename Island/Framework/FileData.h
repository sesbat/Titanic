#pragma once
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp> 
#include <SFML/System/Vector2.hpp>

using json = nlohmann::json;
using namespace std;	

namespace sf {
	void to_json(json& j, const Vector2f& p);
	void from_json(const json& j, Vector2f& p);
	void to_json(json& j, const Vector2i& p);
	void from_json(const json& j, Vector2i& p);
}

namespace ns {
	struct RectangleInfo
	{
		sf::Vector2f size;
		sf::Vector2f pos;
	};
	void to_json(json& j, const RectangleInfo& c);
	void from_json(const json& j, RectangleInfo& c);
}

	struct Item
	{
		string type;
		string path;
		int count = 0;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Item, type, path, count)
	};
	
	struct ObjectData
	{
		string type; // �ٴ�, Ǯ, (����, ����, ����, ��(Ư��)) ���ʹ�
		string path; // ��������Ʈ ����
		sf::Vector2f position;
		sf::Vector2i greedIdx;
		map<string,Item> item;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectData, type, path, position, greedIdx, item)
	};

	struct EditorObjs
	{
		string texPath;
		string uiPaht;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(EditorObjs, texPath, uiPaht)
	};

	struct ItemInfo
	{
		string path;
		int maxCount;
		float weight;
		int width;
		int height;
		int price;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemInfo, path, maxCount, weight, width, height, price)
	};

	struct RequiredItem
	{
		string id;
		int cnt;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(RequiredItem, id, cnt)
	};


	struct CraftingInfo
	{
		vector<RequiredItem> useItem;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CraftingInfo, useItem)
	};
	

	struct UserInfo
	{
		int hp;
		float hungerGuage;
		float thirstGuage;
		float energyGuage;
		int money;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfo, hp, hungerGuage, thirstGuage, energyGuage, money)
	};


	struct InvenInfo
	{
		string Type;
		sf::Vector2i invenPos;
		sf::Vector2i invenGreedPos;
		int cnt;
		string path;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(InvenInfo, Type, invenPos, invenGreedPos, cnt, path)
	};

	struct InvneUseInfo
	{
		string Type;
		int useIdx;
		sf::Vector2i invenPos;
		int cnt;
		string path;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(InvneUseInfo, Type, useIdx, invenPos, cnt, path)
	};