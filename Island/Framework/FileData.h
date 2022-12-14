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

		string uiName;
		int type;
		string description;
		sf::Vector2i toolTipPos;
		sf::Vector2i toolTipSize;
		float tooltipDelay;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemInfo, path, maxCount, weight, width, height, price, uiName, type, description, toolTipPos, toolTipSize, tooltipDelay)
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
	
	struct ArmorStat
	{
		string name;
		int defencePower;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ArmorStat, name, defencePower)
	};

	struct UserInfo
	{
		int hp;
		float hungerGuage;
		float thirstGuage;
		float energyGuage;
		float radGuage;
		int money;
		int ammo;
		int sgAmmo;		//remaining ammo
		int sgAmmo_1up;		//remaining ammo
		int rfAmmo;
		int snAmmo;
		int mb_sgAmmo;
		int mb_snAmmo;
		int mb_rfAmmo;
		int scop_snAmmo;
		int scop_rfAmmo;
		int sr25_Ammo;
		int lastWephon;

		vector<string> clearMaps;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfo, hp, hungerGuage, thirstGuage, energyGuage, radGuage, money, 
			ammo, sgAmmo, sgAmmo_1up, rfAmmo, snAmmo, mb_sgAmmo, mb_snAmmo, mb_rfAmmo,
			scop_snAmmo, scop_rfAmmo, sr25_Ammo,lastWephon, clearMaps)
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

	struct UserStat
	{
		string name;	//valancing variable

		int maxHp;
		float speed;
		float dashSpeed;

		float staminaUpSpeed;
		float staminaDownSpeed;
		float maxStamina;

		float maxHungerGuage;
		float maxThirstGuage;
		float maxEnergyGuage;
		float maxRadiation;

		float hungerDelay;
		float thirstDelay;
		float energyDelay;
		float radiationDelay;

		float radDebuffLevel;
		float radDebuffScale;
		float radDebuffHPDelay;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserStat, name, maxHp, speed, dashSpeed,
			staminaUpSpeed, staminaDownSpeed, maxStamina, maxHungerGuage, maxThirstGuage, maxEnergyGuage, maxRadiation,
			hungerDelay, thirstDelay, energyDelay, radiationDelay, radDebuffLevel, radDebuffScale, radDebuffHPDelay)
	};

	struct GunStat
	{
		string name;	//valancing variable

		float damage;
		float speed;
		float range;
		float reloadDelay;
		int magazine;
		float randDir;
		float zoomRange;
		float shootDelay;
		float shotgunDir;
		string soundPath;

		int cnt;
		
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(GunStat,
			name, damage, speed, range, reloadDelay, magazine, randDir, zoomRange, shootDelay, shotgunDir, soundPath, cnt)
	};
	
	struct ItemEffect
	{
		string name;
		float hp;
		float hungerEffect;
		float thirstEffect;
		float energyEffect;
		float radiationEffect;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemEffect,
			name, hp, hungerEffect, thirstEffect, energyEffect, radiationEffect)
	};

	struct EnemyInfo
	{
		int gun;

		float speed;
		float maxSpeed;
		float dashSpeed;
		int maxHp;
		string type;

		float moveTime;
		float hitTime;
		float patrolTime;
		float hideDelay;

		int patrolBlock;
		float searchDis;
		int damage;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(EnemyInfo,
			gun, speed, maxSpeed, dashSpeed,  maxHp, type, moveTime, hitTime, patrolTime, hideDelay, patrolBlock, searchDis, damage)
	};
	struct ShopItemInfo
	{
		string name;
		int cnt;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShopItemInfo,name, cnt)
	};