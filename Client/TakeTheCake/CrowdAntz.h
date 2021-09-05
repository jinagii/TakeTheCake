#pragma once
#include <windows.h>

class IDXRenderer;
class DXObjectFactory;
class Camera;
class Timer;

class Scene03_InGame;

/// 엔진 데모용 프로세스
class DemoProcess
{
private:
	// 윈도우 초기 위치
	const int m_WndPosX = 0;
	const int m_WndPosY = 0;

	// 화면 크기
	int m_ScreenWidth = 1920;
	int m_ScreenHeight = 1080;

	// 윈도우 관련 
	HWND m_hWnd;
	MSG m_Msg;
	static bool m_IsResizing;

	/// 3D 엔진 추가
	IDXRenderer* m_pRenderer;
	/// static 공부!
	static IDXRenderer* m_pStaticRenderer;

	/// 타이머 등 추가
	Timer* m_pTimer;


private:
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

	/// 씬 로드
	HRESULT LoadAllScene();

};