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
    // ��� Ű�� ���ؼ� �ȴ��� ���·� ����
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
    // �̹� �����ӿ� key�� ��������
    if (GetAsyncKeyState(key) & 0x8000)
    {
        // ���� ���ð��� false(�ȴ������־��ٸ�)
        if (!_keyDown[key])
        {
            // key down ���·� ����
            this->SetKeyDown(key, true);

            // IsOnceKeyDown(key) == true
            return true;
        }

        // �������� ������ �־��ٸ�
        // �ƹ��͵� ����
    }

    // �̹� �����ӿ� key�� ������ �ʾҴٸ� 
    else
    {
        this->SetKeyDown(key, false);
    }

    // IsOnceKeyDown(key) == false
    return false;
}

/// <summary>
/// keyUp�� ���� �ʿ�
/// ���� ���� �۵����� ����
/// </summary>
bool KeyManager::IsOnceKeyUp(int key)
{
    // �̹��� ������ �ʾҰ�
    if (!(GetAsyncKeyState(key) & 0x8000))
    {
        // ������ ���õ� ���� true���
        // �������� up ���¿��ٸ�
        if (this->GetKeyUp(key))

            return true;
    }

    // �̹��� key�� ������ ���¸�
    else
    {
        this->SetKeyUp(key, false);
    }

    return false;
}
/// ���� �ڵ�� �����ʿ�(�۵�X)


bool KeyManager::IsStayKeyDown(int key)
{
    // �̹� ������ Ű�� ������ �ִٸ�
    if (GetAsyncKeyState(key) & 0x8000)
    {
        this->SetKeyDown(key, true);
        return true;
    }

    // �������� �������
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

// Ȯ�� �ϰ� ���� Ű�� ������ �ִ°�
bool KeyManager::InputKey(int vk)
{
    return mKeyStateArr[vk].current;
}

// Ű�� ���� ���� Ȯ���ϰ� ������
bool KeyManager::InputKeyDown(int vk)
{
    return !mKeyStateArr[vk].prev && mKeyStateArr[vk].current;
}

bool KeyManager::InputKeyUp(int key)
{
    return mKeyStateArr[key].prev && !mKeyStateArr[key].current;
}
