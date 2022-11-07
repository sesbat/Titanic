#include "FileManager.h"
#include <fstream>
#include <iostream>

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

}

FileManager::~FileManager()
{
}

void FileManager::LoadAll()
{
	ifstream ep1_f("config/data/episode.json");
	json ep1_d = json::parse(ep1_f);
	episodeMap = ep1_d;
	ep1_f.close();

	ifstream hitbox("config/data/hitBox.json");
	json hit_d = json::parse(hitbox);
	hitBoxMap = hit_d;
	hitbox.close();

	ifstream cookie("config/data/cookieInfo.json");
	json cook_d = json::parse(cookie);
	cookieInfo = cook_d;
	cookie.close();
}

const map<string,MapData>& FileManager::GetEpisode(string episode)
{
    return episodeMap[episode];
}
const HitBoxInfo& FileManager::GetHitBox(string name)
{
	return hitBoxMap[name];
}

void FileManager::SaveData(map<string, map<string, MapData>> newData, string path)
{
	episodeMap = newData;

	json data = newData;

	ofstream ofs(path);
	ofs << data;
	ofs.close();
}

void FileManager::SaveDataEpisode(map<string, MapData> newData, string name)
{
	episodeMap[name].clear();
	episodeMap[name] = newData;


	json data = episodeMap;

	ofstream ofs("config/data/episode.json");
	ofs << data;
	ofs.close();
}
