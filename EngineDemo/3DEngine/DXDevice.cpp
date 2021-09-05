#include "DXDevice.h"

DXDevice* DXDevice::Device = nullptr;

ID3D11Device* DXDevice::g_Device = nullptr;
ID3D11DeviceContext* DXDevice::g_DeviceContext = nullptr;
D3D_DRIVER_TYPE DXDevice::g_D3DDriverType = D3D_DRIVER_TYPE_HARDWARE;
D3D_FEATURE_LEVEL DXDevice::g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
int DXDevice::g_WndStartX = 0;
int DXDevice::g_WndStartY = 0;
int DXDevice::g_ScreenWidth = 0;
int DXDevice::g_ScreenHeight = 0;

DXDevice* DXDevice::GetInstance()
{
	if (Device == nullptr)
	{
		Device = new DXDevice();
	}

	return Device;
}

DXDevice::~DXDevice()
{
	ReleaseCOM(g_DeviceContext);
	ReleaseCOM(g_Device);
}

bool DXDevice::Initialize()
{
	// D3DDevice¿Í deviceContext »ý¼º
	UINT _createDeviceFlags = 0;

#ifdef _DEBUG
	_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT _hr = D3D11CreateDevice(
		0,
		g_D3DDriverType,
		0,
		_createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&g_Device,
		&g_FeatureLevel,
		&g_DeviceContext);

	if (FAILED(_hr))
	{
		MessageBox(0, L"D3D11CreateDevice failed.", 0, 0);
		return false;
	}
	if (g_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"D3DFeatureLevel11 unsupported.", 0, 0);
		return false;
	}

   g_WndStartX = 0;
   g_WndStartY = 0;

	g_ScreenWidth = 1920;
	g_ScreenHeight = 1080;

	return true;
}
