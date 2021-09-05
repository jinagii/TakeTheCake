#include "pch.h"
#include "KeyManager.h"

KeyManager::KeyManager()
    : _keyUp()
    , _keyDown()
{
    Initialize();
}

KeyManager::~KeyManager()
{
}



HRESULT KeyManager::Initialize()
{
    // 모든 키에 대해서 안눌림 상태로 세팅
    for (int i = 0; i < KEYMAX; i++)
    {
        this->SetKeyUp(i, false);
        this->SetKeyDown(i, false);

    }
    return S_OK;
}

void KeyManager::Release()
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
    // 이번 프레임에 key가 눌러졌고
    if (GetAsyncKeyState(key) & 0x8000)
    {
        // 이전 세팅값이 false(안눌러져있었다면)
        if (!_keyDown[key])
        {
            // key down 상태로 세팅
            this->SetKeyDown(key, true);

            // IsOnceKeyDown(key) == true
            return true;
        }

        // 이전에도 눌러져 있었다면
        // 아무것도 안함
    }

    // 이번 프레임에 key가 눌리지 않았다면 
    else
    {
        this->SetKeyDown(key, false);
    }

    // IsOnceKeyDown(key) == false
    return false;
}

/// <summary>
/// keyUp은 수정 필요
/// 현재 현재 작동하지 않음
/// </summary>
bool KeyManager::IsOnceKeyUp(int key)
{
    // 이번에 눌리지 않았고
    if (!(GetAsyncKeyState(key) & 0x8000))
    {
        // 이전에 세팅된 값이 true라면
        // 이전에도 up 상태였다면
        if (this->GetKeyUp(key))

            return true;
    }

    // 이번에 key가 눌려진 상태면
    else
    {
        this->SetKeyUp(key, false);
    }

    return false;
}
/// 위의 코드는 수정필요(작동X)


bool KeyManager::IsStayKeyDown(int key)
{
    // 이번 시점에 키가 눌러져 있다면
    if (GetAsyncKeyState(key) & 0x8000)
    {
        this->SetKeyDown(key, true);
        return true;
    }

    // 눌려있지 않은경우
    return false;
}

void KeyManager::KeyUpdate()
{
    for (int index = 0; index < 255; index++)
    {
        mKeyStateArr[index].prev = mKeyStateArr[index].current;
        mKeyStateArr[index].current = HIWORD(GetAsyncKeyState(index));
    }
}

// 확인 하고 싶은 키가 눌려져 있는거
bool KeyManager::InputKey(int vk)
{
    return mKeyStateArr[vk].current;
}

// 키가 눌린 것을 확인하고 싶을때
bool KeyManager::InputKeyDown(int vk)
{
    return !mKeyStateArr[vk].prev && mKeyStateArr[vk].current;
}

bool KeyManager::InputKeyUp(int key)
{
    return mKeyStateArr[key].prev && !mKeyStateArr[key].current;
}
