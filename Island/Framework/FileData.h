#pragma once
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp> 
#include <SFML/System/Vector2.hpp>

using json = nlohmann::json;
using namespace std;	

enum class FileTypes
{
	EpisodeInfo,
	HitBoxInfo,
	CookieHitBox,
};


namespace sf {
	void to_json(json& j, const Vector2f& p);
	void from_json(const json& j, Vector2f& p);
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
		int pers = 100;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Item, type, path, pers)
	};
	
	struct ObjectData
	{
		string type; // �ٴ�, Ǯ, (����, ����, ����, ��(Ư��)) ���ʹ�
		string path; // ��������Ʈ ����
		sf::Vector2f position;
		vector<Item> item;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectData, type, path, position, item)
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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemInfo, path, maxCount, weight, width, height)
	};
