#pragma once

#include "IRenderer.h"

#include "d3dx11effect.h"
#include "DXDefine.h"

// 클래스 전방 선언
class Camera;
class IDXObjectBase;
class DXObjectFactory;
class BlurFilter;

class Renderer : public IRenderer
{
public:
	Renderer(HWND hWnd, int screenWidth, int screenHeight);
	virtual ~Renderer();

	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update(float dTime) override;

	virtual void BeginRender() override;
	virtual void Render() override;
	virtual void EndRender() override;

	virtual void Finalize() override;

	virtual void SetClientSize(int width, int height) override;
	virtual void OnResize() override;

	virtual void ShowDebugInformation() override;

private:
	// 윈도우 관련
	HWND m_hMainWnd;
	//int m_ClientWidth;
	//int m_ClientHeight;

	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView; // 기본 렌더 타겟
	ID3D11RenderTargetView* m_pRenderTargetViewForBlur; // blur
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencilBuffer; // 공부

	D3D11_VIEWPORT m_ScreenViewport;

	// Quality
	UINT m_4xMSAAQuality;
	bool m_Enable4xMSAA;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	// Mouse Move
	POINT m_PrevMousePos;

	// PostProcessing
	BlurFilter* m_pBlurFilter;
	ID3D11Buffer* m_pScreenQuadVB;
	ID3D11Buffer* m_pScreenQuadIB;
	ID3D11ShaderResourceView* m_pOffScreenSRV;
	ID3D11UnorderedAccessView* m_pOffScreenUAV;
	ID3D11RenderTargetView* m_pOffScreenRTV;
	bool m_BlurFlag;

	void DrawScreenQuad();
	void BuildOffScreenView();
	void BuildScreenQuadGeometryBuffers();
	void BeginBlur();
	void EndBlur();


public:
	// Key입력 관련
	virtual void OnMouseDown(int x, int y) {}
	virtual void OnMouseUp(int x, int y) {}
	virtual void OnMouseMove(int btnState, int x, int y) {}

private:
	// Objects
	std::vector<IDXObjectBase*> m_ObjectList;
	std::vector<IDXObjectBase*> m_HelperObjectList;

	int m_ObjectIndex;
	DXObjectFactory* m_pObjectManager;

	// 템플릿 공부해봤음
// 	template <typename T>
// 	void AddToObjectList(T* object);

};
