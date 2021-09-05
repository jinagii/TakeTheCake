#include "IDXRenderer.h"
#include "Timer.h"
#include "TestScene.h"
#include "DXObjectFactory.h"
#include "DXDevice.h"
#include "DemoProcess.h"

bool DemoProcess::m_IsResizing = false;

IDXRenderer* DemoProcess::m_pStaticRenderer = nullptr;
TestScene* DemoProcess::m_pTestScene = nullptr;

// Renderer가 조절해줘서 현재는 안씀
//void DemoProcess::ResizeWindow()
//{
//	RECT nowRect;
//	DWORD _style = (DWORD)GetWindowLong(m_hWnd, GWL_STYLE);
//	DWORD _exstyle = (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE);
//
//	GetWindowRect(m_hWnd, &nowRect);
//
//	RECT newRect;
//	newRect.left = 0;
//	newRect.top = 0;
//	newRect.right = DXDevice::g_ScreenWidth;
//	newRect.bottom = m_ScreenHeight;
//
//	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);
//
//	// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
//	int _newWidth = (newRect.right - newRect.left);
//	int _newHeight = (newRect.bottom - newRect.top);
//
//	SetWindowPos(m_hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
//		_newWidth, _newHeight, SWP_SHOWWINDOW);
//}

void DemoProcess::UpdateAll()
{
	// 매 프레임 시간 계산
	m_pTimer->Tick();

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	//static int _frmCnt = 0;
	//static float _timeElapsed = 0.0f;
	//_frmCnt++;
	//if ((m_pTimer->TotalTime() - _timeElapsed) >= 1.0f)
	//{
	//	float _fps = (float)_frmCnt;
	//	float _mspf = 1000.0f / _fps;

	//	_frmCnt = 0;
	//	_timeElapsed += 1.0f;

	//	return;
	//}

	m_pTestScene->Update(m_pTimer->DeltaTime()); 		

	m_pRenderer->Update(m_pTimer->DeltaTime());
}

void DemoProcess::RenderAll()
{
	m_pRenderer->BeginRender();
	//m_pRenderer->Render();
	// <- 여기에 렌더 할 씬을 넣는다
	m_pTestScene->Render();

	m_pRenderer->EndRender();
}

HRESULT DemoProcess::Initialize(HINSTANCE hInstance)
{
	DXDevice::Initialize();

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

	//// 윈도 생성
   m_hWnd = CreateWindowW(
      szAppName, szAppName,
		WS_OVERLAPPED| WS_CAPTION| WS_SYSMENU| WS_THICKFRAME| WS_MAXIMIZEBOX,
		DXDevice::g_WndStartX, DXDevice::g_WndStartY, DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight,
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

	Timer* temptimer = new Timer();

	// Factory
	m_pFactory = new DXObjectFactory(DXDevice::g_Device, DXDevice::g_DeviceContext);
	m_pFactory->Initialize();

	// TestScene
	m_pTestScene = new TestScene();
	m_pTestScene->Initialize(m_pFactory);

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
			if(m_pTestScene!= nullptr)
			m_pTestScene->SetLensOnResize(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);
						
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
			m_pStaticRenderer->OnResize();
			return 0;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			m_pTestScene->OnMouseDown(hWnd, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			m_pTestScene->OnMouseUp(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_MOUSEMOVE:
			m_pTestScene->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
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
