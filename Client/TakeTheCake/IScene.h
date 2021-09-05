#pragma once

/// IScene class
///
/// ���ӿ��� ����� ������ ���� ����� ���
/// �߻�ȭ�� Ŭ����
/// 
/// ������ ��� IScene �� ��ӹް�
/// Initialize
/// Release
/// Update
/// Render
/// 
/// �Լ��� override �Ѵ�
/// 
/// First Create	  : 2021.08.18
/// Last Modification : 2021.08.18


class IScene abstract
{
public:
	IScene();
	virtual ~IScene();

	virtual HRESULT Initialize();
	virtual void Release() {}
	virtual void Update(const float dTime) {}
	virtual void Render() {}

	virtual void LoadObject() {}
	virtual void LoadUIObject() {}
	virtual void LoadDebugObject() {}

	virtual void ShowDebugInformation(float dTime) {}

public:
	// Mouse �Է� ����
	virtual void OnMouseDown(HWND hWnd, int x, int y) {}
	virtual void OnMouseUp(int x, int y) {}
	virtual void OnMouseMove(int btnState, int x, int y) {}
	virtual void OnMouseWheel(SHORT wheelValue) {}

	virtual void SetLensOnResize(int width, int height);

	/// �ӽ� ���콺 ����(���߿� �ٸ������� �̵���Ű��)
	// Mouse Move
	static POINT sm_PrevMousePos;

protected:
	// Font
	class DXTKFont* m_pFont;

	////////////////////////////////
	/// ī�޶�
	class Camera* m_pCamera;
};