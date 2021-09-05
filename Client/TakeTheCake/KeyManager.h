#pragma once

/// <summary>
/// KeyManager Class
/// Writer : YoKing
/// 
/// 키보드 입력에 관한 클래스
/// 
/// 키입력 유무 확인 등.
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

	bool IsOnceKeyDown(int key);	// 키 누를 떄
	bool IsOnceKeyUp(int key);		// 키 뗄 때
	bool IsStayKeyDown(int key);	// 키를 누르고 있는 동안

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