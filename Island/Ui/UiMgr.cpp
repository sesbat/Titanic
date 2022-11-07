#include "UiMgr.h"
#include "../Ui/UiObject.h"
#include <iostream>

UiMgr::UiMgr(Scene* scene) : parentScene(scene), enabled(true)
{
}

UiMgr::~UiMgr()
{
   // Release();
}

void UiMgr::Init()
{
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Init();
    }
	Object::Init();
}

void UiMgr::Release()
{
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
        {
            if(obj != nullptr)
                delete obj;
            obj = nullptr;
        }
    }
    uiObjList.clear();
    Object::Release();
}



void UiMgr::SetPos(const Vector2f& pos)
{
    Vector2f delta = pos - position;
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Translate(delta);
    }
    position = pos;
    Object::SetPos(pos);
}

void UiMgr::Update(float dt)
{
    if (!enabled)
        return;

    bool isEvent = false;

    if (nowEvObj != nullptr)
    {
        if (nowEvObj->IsMove() && nowEvObj->IsDrag()) //드래그중이면
        {
            nowEvObj->Update(dt); //얘만 업데이트 이벤트 겹치지 않게 얘만 업데이트
            isEvent = true;
        }
        else if (nowEvObj->IsMoveRight() && nowEvObj->IsDragRight()) //드래그중이면
        {
            nowEvObj->Update(dt); //얘만 업데이트 이벤트 겹치지 않게 얘만 업데이트
            isEvent = true;
        }
    }
    for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++) //역으로 탐색 = 맨 위의 ui먼저 탐색
	{
        for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
        {
            if((*it) != nowEvObj)
                (*it)->EventClear(); //Event 상태 클리어

            if (!isEvent) //이벤트 발생하지 않을때만 업데이트 
            {
                (*it)->Update(dt);
            }
			if ((*it)->GetEvent() && !isEvent)
			{
				isEvent = true;
				nowEvObj = *it; //현재 이벤트 갱신
			}
		}
	}


    if (nowEvObj != nullptr && ((nowEvObj->GetState() == UiState::Exit) || (nowEvObj->GetState() == UiState::Enter))) //현재 이벤트가 Exit이면
    {
        for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
        {
            for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
            {
                if ((*it != nowEvObj))
                {
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::Down)
                    {
                        (*it)->SetState(UiState::Exit); //만약 이전에 눌려져있던게 있다면 Exit
                        (*it)->SetEvent(true);
                    }
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::DownRight)
                    {
                        (*it)->SetState(UiState::Exit); //만약 이전에 눌려져있던게 있다면 Exit
                        (*it)->SetEvent(true);
                    }
                    (*it)->ColorClear(); // 버튼 색상 초기화
                }
            }
        }
        nowEvObj = nullptr;
    }
}

void UiMgr::Draw(RenderWindow& window)
{
    if (!enabled)
        return;
    for (auto& uiObjs : uiObjList)
    {
        for (auto& obj : uiObjs.second)
            obj->Draw(window);
    }
}
