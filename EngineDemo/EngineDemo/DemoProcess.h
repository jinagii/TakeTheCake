#pragma once
#include "DXDefine.h"
#include <windows.h>

class IDXRenderer;
class Timer;
class TestScene;
class DXObjectFactory;

/// ���� ����� ���μ���
class DemoProcess
{
private:
	// ������ ���� 
	HWND m_hWnd;
	MSG m_Msg;
	static bool m_IsResizing;

	/// 3D ���� �߰�
	IDXRenderer* m_pRenderer;
	/// static ����!
	static IDXRenderer* m_pStaticRenderer;

	// Factory
	DXObjectFactory* m_pFactory;


	// TestScene
	static TestScene* m_pTestScene;

	/// Ÿ�̸� �� �߰�
	Timer* m_pTimer;

private:
	// window �ʹ� ũ�� ���� (���� �Ⱦ�)
	//void ResizeWindow();

	// Loop
	void UpdateAll();
	void RenderAll();

public:
	// ���� �ʱ�ȭ
	HRESULT Initialize(HINSTANCE hInstance);
	// ���� ����
	void Loop();
	// ������ ���� delete��
	void Finalize();

	// �޼��� �ڵ鷯 �ݹ�
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};