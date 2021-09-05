#include "DXRenderState.h"

DXRenderState* DXRenderState::RenderState = nullptr;

ID3D11RasterizerState* DXRenderState::g_SolidFrame = nullptr;
ID3D11RasterizerState* DXRenderState::g_WireFrame = nullptr;
ID3D11DepthStencilState* DXRenderState::g_NormalDSS = nullptr;

ID3D11DepthStencilState* DXRenderState::g_DepthStencilState = nullptr;
ID3D11DepthStencilState* DXRenderState::g_OffDepthStencilState = nullptr;

DXRenderState::~DXRenderState()
{
	ReleaseCOM(g_WireFrame);
	ReleaseCOM(g_SolidFrame);
	ReleaseCOM(g_NormalDSS);
	ReleaseCOM(g_DepthStencilState);
	ReleaseCOM(g_OffDepthStencilState);
}

DXRenderState* DXRenderState::GetInstance()
{
	if (RenderState == nullptr)
	{
		RenderState = new DXRenderState();
	}

	return RenderState;
}

void DXRenderState::Initialize(ID3D11Device* pDevice)
{
	D3D11_RASTERIZER_DESC _solidDesc;
	ZeroMemory(&_solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	_solidDesc.FillMode = D3D11_FILL_SOLID;
	_solidDesc.CullMode = D3D11_CULL_BACK;
	_solidDesc.FrontCounterClockwise = false;
	_solidDesc.DepthClipEnable = true;
	HR(pDevice->CreateRasterizerState(&_solidDesc, &g_SolidFrame));

	D3D11_RASTERIZER_DESC _wireframeDesc;
	ZeroMemory(&_wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	_wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	_wireframeDesc.CullMode = D3D11_CULL_BACK;
	_wireframeDesc.FrontCounterClockwise = false;
	_wireframeDesc.DepthClipEnable = true;
	HR(pDevice->CreateRasterizerState(&_wireframeDesc, &g_WireFrame));

	// 폰트용 Depth stencil state
	D3D11_DEPTH_STENCIL_DESC _equalsDesc;
	ZeroMemory(&_equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	_equalsDesc.DepthEnable = true;
	_equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰dla
	_equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	HR(pDevice->CreateDepthStencilState(&_equalsDesc, &g_NormalDSS));


   // DepthStencilState 구조체 초기화
   D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
   ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

   // DepthStencilState 구조체를 작성
   depthStencilDesc.DepthEnable = true;
   depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
   depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

   depthStencilDesc.StencilEnable = true;
   depthStencilDesc.StencilReadMask = 0xFF;
   depthStencilDesc.StencilWriteMask = 0xFF;

   // 픽셀 정면의 스텐실 설정
   depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
   depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
   depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
   depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

   // 픽셀 뒷면의 스텐실 설정
   depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
   depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
   depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
   depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

   HR(pDevice->CreateDepthStencilState(&depthStencilDesc, &g_DepthStencilState));


   D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
   ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

   depthDisabledStencilDesc.DepthEnable = false;
   depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
   depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
   depthDisabledStencilDesc.StencilEnable = true;
   depthDisabledStencilDesc.StencilReadMask = 0xFF;
   depthDisabledStencilDesc.StencilWriteMask = 0xFF;
   depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
   depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
   depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
   depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
   depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
   depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
   depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
   depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

   HR(pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &g_OffDepthStencilState));
}
