#pragma once

/// IScene class
///
/// 게임에서 사용할 씬들의 공통 기능을 모아
/// 추상화한 클래스
/// 
/// 씬들은 모두 IScene 를 상속받고
/// Initialize
/// Release
/// Update
/// Render
/// 
/// 함수를 override 한다
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
	// Mouse 입력 관련
	virtual void OnMouseDown(HWND hWnd, int x, int y) {}
	virtual void OnMouseUp(int x, int y) {}
	virtual void OnMouseMove(int btnState, int x, int y) {}
	virtual void OnMouseWheel(SHORT wheelValue) {}

	virtual void SetLensOnResize(int width, int height);

	/// 임시 마우스 관련(나중에 다른곳으로 이동시키자)
	// Mouse Move
	static POINT sm_PrevMousePos;

protected:
	// Font
	class DXTKFont* m_pFont;

	////////////////////////////////
	/// 카메라
	class Camera* m_pCamera;
};