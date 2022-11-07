#include "AnimationClip.h"
#include "../../Framework/ResourceManager.h"

AnimationFrame::AnimationFrame(const vector<string>& row)
    :texture(RESOURCES_MGR->GetTexture(row[0])), 
    coord(stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4])), 
    origin(stof(row[5]), stof(row[6])), flipX(row[7] == "1"), flipY(row[8] == "1")
{
}
