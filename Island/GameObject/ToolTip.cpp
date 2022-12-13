#include "ToolTip.h"
#include "../GameObject/TextObject.h"
#include "../GameObject/SpriteObject.h"
#include "../Framework/ResourceManager.h"
#include "../Ui/InvenItem.h"
#include <locale.h>

std::string w2s(const std::wstring& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

std::wstring s2w(const std::string& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
}

ToolTip::ToolTip(UiMgr* mgr)
	: Button(uimgr)
{
	SetActive(false);
}

void ToolTip::Release()
{
	for (auto& t : tooltipDatas)
		delete t;

	tooltipDatas.clear();
	Button::Release();
}

void ToolTip::init()
{
	auto itemData = FILE_MGR->GetItemInfo(itemName);

	for (auto& t : tooltipDatas)
		delete t;

	tooltipDatas.clear();

	SetTexture(*RESOURCES_MGR->GetTexture("graphics/100sizeImage.png"), true);
	GetSpriteObj()->SetColor(Color(127, 127, 127, 255));
	GetSpriteObj()->SetSize((Vector2f)itemData.toolTipSize);
	auto toolTipPos = itemPos + (Vector2f)itemData.toolTipPos;

	//if (toolTipPos.x + itemData.toolTipSize.x > 1080)
	//{
	//	toolTipPos.x -= itemData.toolTipSize.x + itemData.toolTipPos.x * -2;
	//}
	//if (toolTipPos.x + itemData.toolTipSize.x > 1080)
	//{
	//	toolTipPos.x -= itemData.toolTipSize.x + itemData.toolTipPos.x * -2;
	//}

	SetPos(toolTipPos);

	SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, s2w(itemData.uiName), false);
	auto txtPos = GetPos() + Vector2f{ GetSpriteObj()->GetGlobalBound().width / 2, 10 };
	GetTextObj()->SetPos(txtPos);
	GetTextObj()->SetOrigin(Origins::MC);

	auto price = new TextObject();
	wstring txtPrice = L"가격: " + to_string(itemData.price);
	price->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txtPrice);
	auto pricePos = GetPos() + Vector2f{ 10, 40 };
	price->SetPos(pricePos);
	price->SetOrigin(Origins::TL);
	tooltipDatas.push_back(price);

	switch (type)
	{
	case ToolTip::ItemType::None:
		break;
	case ToolTip::ItemType::Expendables:
	{
		auto all_ExpendablesData = FILE_MGR->GetItemEffect();
		auto data = all_ExpendablesData[itemName];
		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 0 };

		if (data.hungerEffect != 0)
		{
			auto hunger = new TextObject();
			wstring txt_hunger = L"허기: " + to_string((int)data.hungerEffect);
			hunger->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, data.hungerEffect > 0 ? Color::Green : Color::Red, txt_hunger);
			hunger->SetPos(nextPos);
			hunger->SetOrigin(Origins::TL);
			tooltipDatas.push_back(hunger);
			nextPos.y += 40;
		}
		if (data.thirstEffect != 0)
		{
			auto thirst = new TextObject();
			wstring txt_thirst = L"갈증: " + to_string((int)data.thirstEffect);
			thirst->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, data.thirstEffect > 0 ? Color::Green : Color::Red, txt_thirst);
			thirst->SetPos(nextPos);
			thirst->SetOrigin(Origins::TL);
			tooltipDatas.push_back(thirst);
			nextPos.y += 40;
		}
		if (data.energyEffect != 0)
		{
			auto energy = new TextObject();
			wstring txt_energy = L"피로: " + to_string((int)data.energyEffect);
			energy->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, data.energyEffect > 0 ? Color::Green : Color::Red, txt_energy);
			energy->SetPos(nextPos);
			energy->SetOrigin(Origins::TL);
			tooltipDatas.push_back(energy);
			nextPos.y += 40;
		}

		if (data.radiationEffect != 0)
		{
			auto rad = new TextObject();
			wstring txt_rad = L"방사능: " + to_string((int)data.radiationEffect);
			rad->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, data.radiationEffect < 0 ? Color::Green : Color::Red, txt_rad);
			rad->SetPos(nextPos);
			rad->SetOrigin(Origins::TL);
			tooltipDatas.push_back(rad);
			nextPos.y += 40;
		}

		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{GetPos().x + 10.f  ,nextPos.y};
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
		
	}
	break;
	case ToolTip::ItemType::Stuff:
	{
		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 40 };

		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
	}
	break;
	case ToolTip::ItemType::Wephon:
	{
		auto data = FILE_MGR->GetGunInfo(itemName);
		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 0 };

		auto dmg = new TextObject();
		wstring txt_dmg = L"공격력: " + to_string((int)data.damage);
		dmg->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_dmg);
		dmg->SetPos(nextPos);
		dmg->SetOrigin(Origins::TL);
		tooltipDatas.push_back(dmg);
		nextPos.y += 40;

		auto shootDelay = new TextObject();
		wstring txt_shootDelay = L"연사력: " + to_string(data.shootDelay).substr(0,3);
		shootDelay->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_shootDelay);
		shootDelay->SetPos(nextPos);
		shootDelay->SetOrigin(Origins::TL);
		tooltipDatas.push_back(shootDelay);
		nextPos.y += 40;

		auto speed = new TextObject();
		wstring txt_speed = L"사거리: " + to_string((int)data.speed);
		speed->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_speed);
		speed->SetPos(nextPos);
		speed->SetOrigin(Origins::TL);
		tooltipDatas.push_back(speed);
		nextPos.y += 40;


		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
	}
	break;
	case ToolTip::ItemType::Armor:
	{
		//auto all_ExpendablesData = FILE_MGR->GetItemEffect();
		//auto data = all_ExpendablesData[itemName];

		//auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 0 };

		//auto dmg = new TextObject();
		//wstring txt_dmg = L"체력화복 " + to_string((int)data.);
		//dmg->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_dmg);
		//dmg->SetPos(nextPos);
		//dmg->SetOrigin(Origins::TL);
		//tooltipDatas.push_back(dmg);
		//nextPos.y += 40;


		//auto des = new TextObject();
		//wstring txt_des = L"아이템 설명\n" + itemData.description;
		//des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		//auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		//des->SetPos(desPos);
		//des->SetOrigin(Origins::TL);
		//tooltipDatas.push_back(des);
	}
	break;
	case ToolTip::ItemType::Medical:
	{

		auto all_ExpendablesData = FILE_MGR->GetItemEffect();
		auto data = all_ExpendablesData[itemName];

		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 0 };

		auto hp = new TextObject();
		wstring txt_hp = L"체력화복 " + to_string((int)data.hp);
		hp->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_hp);
		hp->SetPos(nextPos);
		hp->SetOrigin(Origins::TL);
		tooltipDatas.push_back(hp);
		nextPos.y += 40;


		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
	}
	break;
	case ToolTip::ItemType::Ammo:
	{
		
		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 40 };

		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
	}
	break;
	case ToolTip::ItemType::Another:
	{
		auto nextPos = pricePos + Vector2f{ itemData.toolTipSize.x / 2.f, 40 };

		auto des = new TextObject();
		wstring txt_des = L"아이템 설명\n" + itemData.description;
		des->SetText(*RESOURCES_MGR->GetFont("fonts/NotoSansKR-Medium.otf"), 20, Color::White, txt_des);
		auto desPos = Vector2f{ GetPos().x + 10.f  ,nextPos.y };
		des->SetPos(desPos);
		des->SetOrigin(Origins::TL);
		tooltipDatas.push_back(des);
	}
	break;
	default:
		break;
	}

}

void ToolTip::Update(float dt)
{
	if (!enabled)
		return;
	Button::Update(dt);

	if (IsExit())
	{
		SetActive(false);
		//myItem->SetOverlapTimer(0.f);
	}
}

void ToolTip::Draw(RenderWindow & window)
{
	if (!enabled)
		return;
	Button::Draw(window);
	for (auto& t : tooltipDatas)
		t->Draw(window);
}

ToolTip::~ToolTip()
{
	Release();
}

void ToolTip::SetItem(string name)
{
	itemName = name;
}

void ToolTip::SetToolPos(Vector2f v)
{
	itemPos = v;
}
