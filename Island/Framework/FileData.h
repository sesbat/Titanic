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
	struct CircleInfo
	{
		float rad;
		sf::Vector2f pos;
	};
	void to_json(json& j, const CircleInfo& c);
	void from_json(const json& j, CircleInfo& c);
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

namespace ns {
	struct ConvexInfo
	{
		vector<sf::Vector2f> points;
		sf::Vector2f pos;
	};
	void to_json(json& j, const ConvexInfo& c);
	void from_json(const json& j, ConvexInfo& c);
}

namespace ns {
	struct BackInfo
	{
		string path;
		float speed;
	};
	void to_json(json& j, const BackInfo& b);
	void from_json(const json& j, BackInfo& b);
}

	struct MapData
	{
		vector<ns::BackInfo> backInfo; // background path
		string bottomPath;		//bottom block path
		vector<int> bottomPos;	//bottom block draw position
		map<string, vector<sf::Vector2f>> obstacles; //obstacles draw path, draw position
		map<string, vector<sf::Vector2f>> jellys; //obstacles draw path, draw position
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(MapData, backInfo, bottomPath, bottomPos, obstacles, jellys)
	};

	struct HitBoxInfo
	{
		vector<ns::CircleInfo> circles;    // rad, pos
		vector<ns::RectangleInfo> rectangls;  //  size , pos
		vector <ns::ConvexInfo> points; //point
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(HitBoxInfo, circles, rectangls, points)
	};

	struct CookieHitBox
	{
		string type;
		HitBoxInfo hitBox;
		ns::RectangleInfo bottom;
		
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CookieHitBox, type, hitBox, bottom)
	};

