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
		string type; // 바닥, 풀, (나무, 바위, 상자, 벽(특수)) 에너미
		string path; // 스프라이트 파일
		string uiPath; // 에디터 버튼 스프라이트
		sf::Vector2f position;
		vector<ns::RectangleInfo> hitBox;  //  size , pos
		ns::RectangleInfo bottom;

		vector<Item> item;
		vector<Item> randomItem;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectData, type, path, uiPath, position, hitBox, bottom, item, randomItem)
	};

	struct EditorObjs
	{
		string texPath;
		string uiPaht;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(EditorObjs, texPath, uiPaht)
	};
