#pragma once

#include "DXDefine.h"

class Renderer;

/// ������ ����� ������ �Լ��� 
/// ������ Renderer���θ� �𸣰� �ѹ��� wrapping
/// (dll�̶� ����� ����?!)
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
	// Key�Է� ����
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

private:
	Renderer* m_pCoRenderer; //
};