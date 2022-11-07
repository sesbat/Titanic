#pragma once
#include "../3rd/SingleTon.h"
#include <SFML/Audio.hpp>
#include <list>

using namespace std;
using namespace sf;

class SoundManager : public Singleton<SoundManager>
{
private:
	list<Sound*> playing;
	list<Sound*> waiting;
	float volume;
	string nowBgm;
public:
	const int TotalChannels;
	SoundManager();
	~SoundManager();
	void Init();
	void Release();
	void Play(string id, bool loop = false);
	void StopAll();
	void Update();
	void SetNowBgm(string str) { nowBgm = str; }
	string GetNowBgm() { return nowBgm; }
};

#define SOUND_MGR (SoundManager::GetInstance())