#include "pch.h"
#include "DXTKFont.h"
#include "DXRenderState.h"
#include "Camera.h"
#include "IScene.h"

POINT IScene::sm_PrevMousePos = { 0, 0 };



IScene::IScene()
	: m_pFont(nullptr)
	, m_pCamera(nullptr)
{
	Initialize();
}

IScene::~IScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pFont);
}

HRESULT IScene::Initialize()
{
	if (!m_pCamera)
	{
		m_pCamera = new Camera();
		m_pCamera->LookAt(Vector3(8.0f, 8.0f, -8.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
	}

	if (!m_pFont)
	{
		m_pFont = new DXTKFont();
		m_pFont->Initialize(DXDevice::g_Device, DXRenderState::g_SolidFrame, DXRenderState::g_NormalDSS);

	}
	
	SetLensOnResize(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight);

	return S_OK;
}

void IScene::SetLensOnResize(int width, int height)
{
	m_pCamera->SetLens(0.25f * MathHelper::Pi, static_cast<float>(width) / height, 1.0f, 10000.0f);
}

