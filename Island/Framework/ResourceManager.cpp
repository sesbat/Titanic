#include "ResourceManager.h"
#include "info.h"

using namespace std;

string ResourceManager::FilePath("config/resources.csv");

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::ReleaseAll()
{
    for (auto it : texMap)
    {
        delete it.second;
    }
    texMap.clear();
    for (auto it : fontMap)
    {
        delete it.second;
    }
    fontMap.clear();
    for (auto it : soundMap)
    {
        delete it.second;
    }
    soundMap.clear();
}


bool ResourceManager::LoadAll()
{
    ReleaseAll();
    rapidcsv::Document doc(FilePath, rapidcsv::LabelParams(0, -1));

    auto ids = doc.GetColumn<string>(0);
    auto types = doc.GetColumn<int>(1);

    for (int i = 0; i < doc.GetRowCount(); i++)
    {
        if (!Load((ResourcesTypes)types[i], ids[i]))
            return false;
    }

    return true;
}
bool ResourceManager::Load(ResourcesTypes type, string id)
{
    switch (type)
    {
    case ResourcesTypes::Texture:
        return LoadTexture(id);
    case ResourcesTypes::Font:
        return LoadFont(id);
    case ResourcesTypes::SoundBuffer:
        return LoadSoundBuffer(id);
    case ResourcesTypes::AnimationClip:
        return LoadAnimationClip(id);
    }

    // Error Msg
    return false;
}

bool ResourceManager::LoadTexture(string id)
{
    if (texMap.find(id) != texMap.end())
    {
        return false;
    }
    auto texture = new Texture();
    if (!texture->loadFromFile(id))
    {
        delete texture;
        return false;
    }
    texMap.insert({id, texture});
    return true;
}

bool ResourceManager::LoadFont(string id)
{
    if (fontMap.find(id) != fontMap.end())
    {
        return false;
    }
    auto font = new Font();
    if (!font->loadFromFile(id))
    {
        delete font;
        return false;
    }
    fontMap.insert({ id, font });
    return true;
}

bool ResourceManager::LoadSoundBuffer(string id)
{
    if (soundMap.find(id) != soundMap.end())
    {
        return false;
    }
    auto buffer = new SoundBuffer();
    if (!buffer->loadFromFile(id))
    {
        delete buffer;
        return false;
    }
    soundMap.insert({ id, buffer });
    return true;
}

bool ResourceManager::LoadAnimationClip(string id)
{
    rapidcsv::Document csv(id);
    AnimationClip* clip = new AnimationClip();
    auto rowClip = csv.GetRow<string>(0);

    clip->id = rowClip[0];
    clip->loopType = (LoopTypes)stoi(rowClip[1]);
    clip->fps = stoi(rowClip[2]);

    for (int i = 3; i < csv.GetRowCount(); i++)
    {
        clip->frames.push_back(csv.GetRow<string>(i));
    }

    animationClipMap.insert({ clip->id, clip });
    return true;
}

Texture* ResourceManager::GetTexture(string id)
{
    auto it = texMap.find(id);
    if (it == texMap.end())
        return nullptr;
    return it->second;
}

Font* ResourceManager::GetFont(string id)
{
    auto it = fontMap.find(id);
    if (it == fontMap.end())
        return nullptr;
    return it->second;
}

SoundBuffer* ResourceManager::GetSoundBuffer(string id)
{
    auto it = soundMap.find(id);
    if (it == soundMap.end())
        return nullptr;
    return it->second;
}

AnimationClip* ResourceManager::GetAnimationClip(string id)
{
    auto it = animationClipMap.find(id);
    if (it == animationClipMap.end())
        return nullptr;
    return it->second;
}
