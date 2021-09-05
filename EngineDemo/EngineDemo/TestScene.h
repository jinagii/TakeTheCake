#pragma once

#include "IScene.h"
#include "DXTKFont.h"

class IDXObjectBase;
class DXTKFont;
class Camera;
class DebugCircle;
class Navigation;
//class Sky;

class TestScene : public IScene
{
public:
	TestScene();
	virtual ~TestScene();

	virtual void Initialize(DXObjectFactory* pFactory);
	virtual void Update(float dTime);
	virtual void Render();

	void Finalize();

public:
	//void LocateObjects(DXObjectFactory* pFactory); // 오브젝트들 배치, 현재는 일단 중점에만 배치

	void ShowDebugInformation();
	
	// Key입력 관련
	

	void OnMouseDown(HWND hWnd, int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

	void SetLensOnResize(int width, int height);
	
private:
	// Mouse Move
	POINT m_PrevMousePos;


private:

	// Font
	DXTKFont* m_pFont;

	Camera* m_pCamera;


	unordered_map<string, IDXObjectBase*> m_Objects_um;
	unordered_map<string, IDXObjectBase*>::iterator m_ObjectsIter;

	unordered_map<string, IDXObjectBase*> m_HelperObject_um;

   vector<DebugCircle*> m_DebugCircles;

	float m_DeltaTime;

	Vector3 m_PlayerPastPosition;

	Navigation* m_pNavigationMesh;
	//Sky* m_pSky;
};

