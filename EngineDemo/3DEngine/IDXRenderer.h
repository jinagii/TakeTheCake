#pragma once

#include "DXDefine.h"

class Renderer;

/// 유저가 사용할 렌더러 함수들 
/// 유저가 Renderer내부를 모르게 한번더 wrapping
/// (dll이랑 비슷한 원리?!)
/// 2021.8.3 JinHak

class IDXRenderer
{
public:
	IDXRenderer(int hWnd, int screenWidth, int screenHeight);
	~IDXRenderer();

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Update(float dTime);

	void BeginRender();
	void Render();
	void EndRender();

	void Finalize();

	void SetClientSize(int width, int height);
	void OnResize();

	void ShowDebugInformation();

public:
	// Key입력 관련
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

private:
	Renderer* m_pCoRenderer; //
};