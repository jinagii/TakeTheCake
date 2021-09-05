#pragma once

///// �ܼ�â ����
//// ���� �ڵ� ȯ���� ���
//#ifdef UNICODE	 
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//
//// ��Ƽ����Ʈ ȯ���� ���
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

/// ���� ����ϱ� ����
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



/// Singleton ������Ʈ
#include "SingletonBase.h"
#include "ObjectFactory.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "SoundManager.h"


#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }
//#define SAFE_RELEASE(p) { if(p) { delete p; p = nullptr; } }

/// define �̸� ����
#include "def.h"