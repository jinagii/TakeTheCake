#pragma once

/// <summary>
/// KeyManager Class
/// Writer : YoKing
/// 
/// Ű���� �Է¿� ���� Ŭ����
/// 
/// Ű�Է� ���� Ȯ�� ��.
/// 
/// First Create	  : 2021.08.19
/// Last Modification : 2021.08.19
/// </summary>


#include "SingletonBase.h"

#define KEYMAX		256

class KeyState
{
public:
	KeyState() : current(false), prev(false) {};
	~KeyState() {};

public:
	bool current;
	bool prev;
};

class KeyManager : public SingletonBase<KeyManager>
{
private:
	bool _keyUp[KEYMAX];
	bool _keyDown[KEYMAX];


public:
	KeyManager();
	~KeyManager();

	HRESULT Initialize() override;
	void Release();

	bool IsOnceKeyDown(int key);	// Ű ���� ��
	bool IsOnceKeyUp(int key);		// Ű �� ��
	bool IsStayKeyDown(int key);	// Ű�� ������ �ִ� ����

	bool GetKeyUp(int i) { return _keyUp[i]; }
	bool GetKeyYDown(int i) { return _keyDown[i]; }

	void SetKeyUp(int key, bool state) { _keyUp[key] = state; }
	void SetKeyDown(int key, bool state) { _keyDown[key] = state; }

	/////////////////////////////////////////////////////////////////
	KeyState mKeyStateArr[KEYMAX];

	void KeyUpdate();
	bool InputKey(int vk);
	bool InputKeyDown(int vk);
	bool InputKeyUp(int key);
};