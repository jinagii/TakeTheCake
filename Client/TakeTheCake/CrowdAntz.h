#pragma once
#include <windows.h>

class IDXRenderer;
class DXObjectFactory;
class Camera;
class Timer;

class Scene03_InGame;

/// ���� ����� ���μ���
class DemoProcess
{
private:
	// ������ �ʱ� ��ġ
	const int m_WndPosX = 0;
	const int m_WndPosY = 0;

	// ȭ�� ũ��
	int m_ScreenWidth = 1920;
	int m_ScreenHeight = 1080;

	// ������ ���� 
	HWND m_hWnd;
	MSG m_Msg;
	static bool m_IsResizing;

	/// 3D ���� �߰�
	IDXRenderer* m_pRenderer;
	/// static ����!
	static IDXRenderer* m_pStaticRenderer;

	/// Ÿ�̸� �� �߰�
	Timer* m_pTimer;


private:
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

	/// �� �ε�
	HRESULT LoadAllScene();

};