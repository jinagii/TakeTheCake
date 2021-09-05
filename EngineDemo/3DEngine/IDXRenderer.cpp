
// 구현에 필요한 것들
#include <windows.h>
#include "Renderer.h"

#include "IDXRenderer.h"

//Renderer* g_pRenderer = nullptr; // 이렇게 g_ 전역으로 하거나 private으로 가지고있거나

IDXRenderer::IDXRenderer(int hWnd, int screenWidth, int screenHeight)
{
	HWND _hWnd = (HWND)hWnd; // 핸들윈도우를 캐스팅해서 보내줘도된다. 헤더에서 <windows.h>를 모르게?

	m_pCoRenderer = new Renderer(_hWnd, screenWidth, screenHeight);
	//g_pRenderer = new Renderer(_hWnd, screenWidth, screenHeight);
}

IDXRenderer::~IDXRenderer()
{
	delete m_pCoRenderer;
	m_pCoRenderer = nullptr;
}

void IDXRenderer::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pCoRenderer->Initialize(pDevice, pDeviceContext);
}

void IDXRenderer::Update(float dTime)
{
	m_pCoRenderer->Update(dTime);
}

void IDXRenderer::BeginRender()
{
	m_pCoRenderer->BeginRender();
}

void IDXRenderer::Render()
{
	m_pCoRenderer->Render();
}

void IDXRenderer::EndRender()
{
	m_pCoRenderer->EndRender();
}

void IDXRenderer::Finalize()
{
	m_pCoRenderer->Finalize();
}

void IDXRenderer::SetClientSize(int width, int height)
{
	m_pCoRenderer->SetClientSize(width, height);
}

void IDXRenderer::OnResize()
{
	m_pCoRenderer->OnResize();
}

void IDXRenderer::ShowDebugInformation()
{
	m_pCoRenderer->ShowDebugInformation();
}

void IDXRenderer::OnMouseDown(int x, int y)
{
	m_pCoRenderer->OnMouseDown(x, y);
}

void IDXRenderer::OnMouseUp(int x, int y)
{
	m_pCoRenderer->OnMouseUp(x, y);
}

void IDXRenderer::OnMouseMove(int btnState, int x, int y)
{
	m_pCoRenderer->OnMouseMove(btnState, x, y);
}

