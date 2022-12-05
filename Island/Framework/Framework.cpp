#include "Framework.h"
#include "info.h"
#include "InputMgr.h"
#include "../3rd/SingleTon.h"
#include "../Scens/SceneManager.h"
#include "ResourceManager.h"
#include "../DataTable/DataTableMGR.h"
#include "SoundManager.h"
#include "FileManager.h"


Framework::Framework()
    :timeScale(1.f)
{
    windowSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
    window.create(VideoMode(windowSize.x, windowSize.y), "Game");
}

Framework::~Framework()
{
    SCENE_MGR->Destroy();
    FILE_MGR->Destroy();
    SOUND_MGR->Destroy();
    RESOURCES_MGR->Destroy();
}

float Framework::GetDT() const
{
    return deltaTime.asSeconds() * timeScale;
}

float Framework::GetRealDT() const
{
    return deltaTime.asSeconds();
}

const Vector2i& Framework::GetWindowSize() const
{
    return windowSize;
}

bool Framework::Init()
{
    InputMgr::Init();
    RESOURCES_MGR->LoadAll();
    FILE_MGR->LoadAll();
    SOUND_MGR->Init();
   // DATATABLE_MGR->Init();
    SCENE_MGR->Init();
    
    return true;
}

bool Framework::Do()
{
    while (window.isOpen())
    {
        deltaTime = clock.restart();
       
        float dt = GetDT();
        //dt /= 10;
        InputMgr::Update(dt);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            InputMgr::ProcessInput(event);
        }

        SCENE_MGR->Update(dt);
        SOUND_MGR->Update();

        window.clear();
        SCENE_MGR->Draw(window);
        window.display();
    }

    return true;
}

#define DATATABLE_MGR (DataTableMgr::GetInstance())