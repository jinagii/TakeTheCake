#include "pch.h"

/// DX 엔진
#include "DXDevice.h"
#include "IDXRenderer.h" // interface로 함수만 가져다쓸때 renderer를 include한다.
#include "DXObjectFactory.h"
#include "Camera.h"

/// 타이머
#include "Timer.h"

/// 클라이언트 오브젝트
#include "AntBase.h"

/// 씬
#include "Scene01_Intro.h"
#include "Scene02_CutScene.h"
#include "Scene03_InGame.h"
#include "Scene04_GameClear.h"
#include "Scene05_GameOver.h"

#include "CrowdAntz.h"

bool DemoProcess::m_IsResizing = false;
IDXRenderer* DemoProcess::m_pStaticRenderer = nullptr;

void DemoProcess::UpdateAll()
{
	// 매 프레임 시간 계산
	m_pTimer->Tick();

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	static int _frmCnt = 0;
	static float _timeElapsed = 0.0f;
	_frmCnt++;
	if ((m_pTimer->TotalTime() - _timeElapsed) >= 1.0f)
	{
		float _fps = (float)_frmCnt;
		float _mspf = 1000.0f / _fps;

		_frmCnt = 0;
		_timeElapsed += 1.0f;

	}

	KeyManager::GetInstance()->KeyUpdate();

	SceneManager::GetInstance()->Update(m_pTimer->DeltaTime());

	m_pRenderer->Update(m_pTimer->DeltaTime());

}

void DemoProcess::RenderAll()
{
	m_pRenderer->BeginRender();

	/// 렌더할 부분들
	//////////////////////////////////

	SceneManager::GetInstance()->Render();

	//////////////////////////////////

	m_pRenderer->EndRender();
}

HRESULT DemoProcess::Initialize(HINSTANCE hInstance)
{
	DXDevice::Initialize();

	DXDevice::g_WndStartX = m_WndPosX;
	DXDevice::g_WndStartY = m_WndPosY;


	wchar_t szAppName[] = L"Demo for 3D Engine";

	// 윈도우 정보 입력
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szAppName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// 윈도 클래스 등록
	RegisterClassExW(&wcex);

	// 윈도 생성
	m_hWnd = CreateWindowW(
		szAppName, szAppName,
		WS_OVERLAPPEDWINDOW,
		m_WndPosX, m_WndPosY, DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight,
		nullptr, nullptr, hInstance, NULL);

	if (!m_hWnd) return E_FAIL;

	m_pRenderer = new IDXRenderer((int)m_hWnd, DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);
	m_pRenderer->Initialize(DXDevice::g_Device, DXDevice::g_DeviceContext);

	m_pStaticRenderer = m_pRenderer;

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	// 타이머 초기화
	m_pTimer = new Timer();
	m_pTimer->Reset();

	/// UI(Texture2D) 로드
	UIManager::GetInstance()->Initialize();

	/// Sound 로드
	SoundManager::GetInstance()->Initialize();

	/// 씬 로드
	LoadAllScene();

	/// 시작씬 설정
	SceneManager::GetInstance()->ChangeScene(SCENE_01_INTRO);

	return S_OK;
}

void DemoProcess::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;
			DispatchMessage(&m_Msg);
		}
		else // 게임 루프
		{
			UpdateAll();
			RenderAll();
		}
	}
}

void DemoProcess::Finalize()
{
	m_pRenderer->Finalize();

	delete m_pRenderer;
	m_pRenderer = nullptr;

}

LRESULT CALLBACK DemoProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_ACTIVATE:
	{
		return 0;
	}
	case WM_COMMAND:
	{
		break;
	}
	case WM_SIZE:
	{
		m_pStaticRenderer->SetClientSize(LOWORD(lParam), HIWORD(lParam));
		m_pStaticRenderer->OnResize();

		if (SceneManager::GetInstance()->GetCurrentScene() != nullptr)
			SceneManager::GetInstance()->GetCurrentScene()->SetLensOnResize(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

		return 0;
	}
	case WM_ENTERSIZEMOVE:
	{
		m_IsResizing = true;

		return 0;
	}
	case WM_EXITSIZEMOVE:
	{
		m_IsResizing = false;
		SceneManager::GetInstance()->GetCurrentScene()->SetLensOnResize(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

		return 0;
	}
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		SceneManager::GetInstance()->GetCurrentScene()->OnMouseDown(hWnd, LOWORD(lParam), HIWORD(lParam));

		return 0;
	}
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		SceneManager::GetInstance()->GetCurrentScene()->OnMouseUp(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MOUSEMOVE:
		SceneManager::GetInstance()->GetCurrentScene()->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_MOUSEWHEEL:
	{
		SceneManager::GetInstance()->GetCurrentScene()->OnMouseWheel((SHORT)HIWORD(wParam));
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}

	return  DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT DemoProcess::LoadAllScene()
{
	IScene* scene;

	/// 인트로 씬 생성
	scene = new Scene01_Intro();
	SceneManager::GetInstance()->AddScene(SCENE_01_INTRO, scene);

	/// 컷 씬 생성
	scene = new Scene02_CutScene();
	SceneManager::GetInstance()->AddScene(SCENE_02_CUTSCENE, scene);

	/// 인게임 씬 생성
	scene = new Scene03_InGame();
	SceneManager::GetInstance()->AddScene(SCENE_03_INGAME, scene);

	/// 게임 클리어 씬 생성
	scene = new Scene04_GameClear();
	SceneManager::GetInstance()->AddScene(SCENE_04_CLEAR, scene);

	/// 게임 오버 씬 생성
	scene = new Scene05_GameOver();
	SceneManager::GetInstance()->AddScene(SCENE_05_OVER, scene);


	return S_OK;
}