#pragma once
#include "DXDefine.h"
#include <windows.h>

class IDXRenderer;
class Timer;
class TestScene;
class DXObjectFactory;

/// 엔진 데모용 프로세스
class DemoProcess
{
private:
	// 윈도우 관련 
	HWND m_hWnd;
	MSG m_Msg;
	static bool m_IsResizing;

	/// 3D 엔진 추가
	IDXRenderer* m_pRenderer;
	/// static 공부!
	static IDXRenderer* m_pStaticRenderer;

	// Factory
	DXObjectFactory* m_pFactory;


	// TestScene
	static TestScene* m_pTestScene;

	/// 타이머 등 추가
	Timer* m_pTimer;

private:
	// window 초반 크기 조정 (현재 안씀)
	//void ResizeWindow();

	// Loop
	void UpdateAll();
	void RenderAll();

public:
	// 게임 초기화
	HRESULT Initialize(HINSTANCE hInstance);
	// 메인 루프
	void Loop();
	// 종료전 정리 delete등
	void Finalize();

	// 메세지 핸들러 콜백
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};