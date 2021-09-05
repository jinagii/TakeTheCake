#pragma once
#include "DXDefine.h"

/// Device 정보를 담은 클래스(Device, DeviceContext etc.)
/// 2021.8.3 JinHak

class DXDevice
{
public:
	static DXDevice* GetInstance();
	~DXDevice();

	static bool Initialize();

	static ID3D11Device* g_Device;
	static ID3D11DeviceContext* g_DeviceContext;
	static D3D_DRIVER_TYPE g_D3DDriverType;
	static D3D_FEATURE_LEVEL g_FeatureLevel;
	static int g_WndStartX;
	static int g_WndStartY;
	static int g_ScreenWidth;
	static int g_ScreenHeight;

private:
	DXDevice() {};
	static DXDevice* Device;
};

