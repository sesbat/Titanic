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
        if (nowEvObj->IsMove() && nowEvObj->IsDrag()) //�巡�����̸�
        {
            nowEvObj->Update(dt); //�길 ������Ʈ �̺�Ʈ ��ġ�� �ʰ� �길 ������Ʈ
            isEvent = true;
        }
        else if (nowEvObj->IsMoveRight() && nowEvObj->IsDragRight()) //�巡�����̸�
        {
            nowEvObj->Update(dt); //�길 ������Ʈ �̺�Ʈ ��ġ�� �ʰ� �길 ������Ʈ
            isEvent = true;
        }
    }
    for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++) //������ Ž�� = �� ���� ui���� Ž��
	{
        for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
        {
            if((*it) != nowEvObj)
                (*it)->EventClear(); //Event ���� Ŭ����

            if (!isEvent) //�̺�Ʈ �߻����� �������� ������Ʈ 
            {
                (*it)->Update(dt);
            }
			if ((*it)->GetEvent() && !isEvent)
			{
				isEvent = true;
				nowEvObj = *it; //���� �̺�Ʈ ����
			}
		}
	}


    if (nowEvObj != nullptr && ((nowEvObj->GetState() == UiState::Exit) || (nowEvObj->GetState() == UiState::Enter))) //���� �̺�Ʈ�� Exit�̸�
    {
        for (auto uiObjs = uiObjList.rbegin(); uiObjs != uiObjList.rend(); uiObjs++)
        {
            for (auto it = (*uiObjs).second.rbegin(); it != (*uiObjs).second.rend(); it++)
            {
                if ((*it != nowEvObj))
                {
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::Down)
                    {
                        (*it)->SetState(UiState::Exit); //���� ������ �������ִ��� �ִٸ� Exit
                        (*it)->SetEvent(true);
                    }
                    if ((*it)->GetState() == UiState::Stay || (*it)->GetState() == UiState::DownRight)
                    {
                        (*it)->SetState(UiState::Exit); //���� ������ �������ִ��� �ִٸ� Exit
                        (*it)->SetEvent(true);
                    }
                    (*it)->ColorClear(); // ��ư ���� �ʱ�ȭ
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
