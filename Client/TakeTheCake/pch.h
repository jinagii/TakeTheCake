#pragma once

///// 콘솔창 띄우기
//// 유니 코드 환경일 경우
//#ifdef UNICODE	 
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//
//// 멀티바이트 환경일 경우
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

/// 사운드 사용하기 위해
#include <Digitalv.h>
#pragma comment(lib,"winmm")
#include <stdlib.h>
#include "Sound.h"


/// C++ Lib
#include <iostream>

/// STL
#include <vector>
#include <string>
#include <map>

/// DX
#include "DXDevice.h"   // Device and DeviceContext
//#include <SimpleMath.h> // DirectX::SimpleMath

/// CommonMacro
#include "CommonMacroFunction.h"



/// Singleton 오브젝트
#include "SingletonBase.h"
#include "ObjectFactory.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "SoundManager.h"


#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }
//#define SAFE_RELEASE(p) { if(p) { delete p; p = nullptr; } }

/// define 이름 정의
#include "def.h"