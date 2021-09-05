#pragma once

#include "DXDefine.h"
#include <windows.h>

/// 렌더러 인터페이스
/// 2021.8.3 JinHak

class IRenderer
{
public:
	IRenderer() {}
	virtual ~IRenderer() {}

	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) abstract;
	virtual void Update(float dTime) abstract;

	virtual void BeginRender() abstract;
	virtual void Render() abstract;
	virtual void EndRender() abstract;

	virtual void Finalize() abstract;

	virtual void SetClientSize(int width, int height) abstract;
	virtual void OnResize() abstract;

	virtual void ShowDebugInformation() abstract;

public:
	// Key입력 관련
	virtual void OnMouseDown(int x, int y) abstract;
	virtual void OnMouseUp(int x, int y) abstract;
	virtual void OnMouseMove(int btnState, int x, int y) abstract;
};