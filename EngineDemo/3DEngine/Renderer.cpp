
#include "DXDevice.h"

#include "Effect.h"
#include "DXRenderState.h"
//#include "DXDevice.h"
// Objects---------------------
#include "IDXObjectBase.h"
#include "Box.h"
//#include "Axis.h"
//#include "Grid.h"
#include "Skull.h"
#include "Camera.h"
#include "MeshObject.h"
// ----------------------------
#include "DXObjectFactory.h"

#include "BlurFilter.h"
#include "GeometryGenerator.h"

#include "Renderer.h"


Renderer::Renderer(HWND hWnd, int screenWidth, int screenHeight)
   : m_hMainWnd(hWnd),
   //m_ClientWidth(screenWidth), m_ClientHeight(screenHeight),
   m_pSwapChain(nullptr),
   m_pRenderTargetView(nullptr),
   m_pDepthStencilBuffer(nullptr),
   m_pDepthStencilView(nullptr),
   m_4xMSAAQuality(0), m_Enable4xMSAA(false),
   m_ObjectIndex(0), m_BlurFlag(false)

{
   ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

   //m_pCamera = new Camera();
}

Renderer::~Renderer()
{
   // Finalize에서 처리
}

void Renderer::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
   // D3DDevice와 deviceContext 생성
   //DXDevice::Initialize();
   m_pDevice = pDevice;
   m_pDeviceContext = pDeviceContext;

   // Multicampling 몇단계까지 되는지 체크
   HR(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality));
   assert(m_4xMSAAQuality > 0);

   // SwapChain 설정하기
   DXGI_SWAP_CHAIN_DESC _scd;
   _scd.BufferDesc.Width = DXDevice::g_ScreenWidth;
   _scd.BufferDesc.Height = DXDevice::g_ScreenHeight;
   _scd.BufferDesc.RefreshRate.Numerator = 60;
   _scd.BufferDesc.RefreshRate.Denominator = 1;
   _scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   _scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
   _scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

   if (m_Enable4xMSAA == true) // Use 4X MSAA? 
   {
      _scd.SampleDesc.Count = 4;
      _scd.SampleDesc.Quality = m_4xMSAAQuality - 1;
   }
   else // No MSAA
   {
      _scd.SampleDesc.Count = 1;
      _scd.SampleDesc.Quality = 0;
   }

   _scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   _scd.BufferCount = 1;
   _scd.OutputWindow = m_hMainWnd;
   _scd.Windowed = true;
   _scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   _scd.Flags = 0;

   // D3디바이스로부터 dxgi디바이스를 얻어옴
   IDXGIDevice* _pXGIDevice = nullptr;
   HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&_pXGIDevice));

   // dxgi디바이스로부터 dxgi어댑터를 얻어옴
   IDXGIAdapter* _pXGIAdapter = nullptr;
   HR(_pXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&_pXGIAdapter));

   // dxgi어댑터로부터 dxgi팩토리를 얻어옴
   IDXGIFactory* _pDXGIFactory = nullptr;
   HR(_pXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&_pDXGIFactory));

   // Swapchain 생성
   HR(_pDXGIFactory->CreateSwapChain(m_pDevice, &_scd, &m_pSwapChain));

   ReleaseCOM(_pXGIDevice);
   ReleaseCOM(_pXGIAdapter);
   ReleaseCOM(_pDXGIFactory);

   OnResize();

   // PostProcessing
   //BuildScreenQuadGeometryBuffers();
   //BuildOffScreenView();
   //m_pBlurFilter = new BlurFilter();
   //m_pBlurFilter->Init(m_pDevice, DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM);



   DXRenderState::Initialize(m_pDevice);
   StaticEffect::InitializeAll(m_pDevice);
   InputLayout::InitializeAll(m_pDevice);


}


/// 굳이 필요가 없는듯?
void Renderer::Update(float dTime)
{


   /*if (GetAsyncKeyState('B') & 0x0001)
      m_BlurFlag = !m_BlurFlag;*/


}

void Renderer::BeginRender()
{
   assert(m_pDeviceContext);

   //if (m_BlurFlag == true)
   //{
   //	BeginBlur();
   //}
   //else
   //{
      // RenderTargetView 초기화
   m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,
      reinterpret_cast<const float*>(&Colors::DarkSlateGray));

   // DepthStencil 초기화
   m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
      D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
   //}

}

void Renderer::Render()
{


}

void Renderer::EndRender()
{
   m_pDeviceContext->OMSetDepthStencilState(DXRenderState::g_NormalDSS, 0);
   // PostProcessing
   //if (m_BlurFlag == true)
   //{
   //	EndBlur();
   //}


   m_pDeviceContext->RSSetState(0);

   assert(m_pSwapChain);

   // 프리젠트 - 백버퍼 교환해주기
   HR(m_pSwapChain->Present(0, 0));
}

void Renderer::Finalize()
{
   StaticEffect::FinalizeAll();

   SafeDelete(m_pObjectManager);


   ReleaseCOM(m_pRenderTargetView);
   ReleaseCOM(m_pDepthStencilView);
   ReleaseCOM(m_pSwapChain);
   ReleaseCOM(m_pDepthStencilBuffer);

   if (m_pDeviceContext)
   {
      m_pDeviceContext->ClearState();
   }


}

void Renderer::SetClientSize(int width, int height)
{
   //m_ClientWidth = width;
   //m_ClientHeight = height;
   DXDevice::g_ScreenWidth = width;
   DXDevice::g_ScreenHeight = height;
}

void Renderer::OnResize() /// demo에서도 해주는데 이것은 init외에 프로세스 중에 조절?
{  

   assert(m_pDeviceContext);
   assert(m_pDevice);
   assert(m_pSwapChain);

   // 기존뷰와 버퍼를 없애고 resize 시작
   ReleaseCOM(m_pRenderTargetView);
   ReleaseCOM(m_pDepthStencilView);
   ReleaseCOM(m_pDepthStencilBuffer);

   // 스왑체인과 렌더타겟뷰 재설정
   HR(m_pSwapChain->ResizeBuffers(1, DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
   ID3D11Texture2D* _backBuffer;
   HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer)));
   HR(m_pDevice->CreateRenderTargetView(_backBuffer, 0, &m_pRenderTargetView));
   ReleaseCOM(_backBuffer);

   // 다시 만들기
   D3D11_TEXTURE2D_DESC _depthStencilBuffer;
   _depthStencilBuffer.Width = DXDevice::g_ScreenWidth;
   _depthStencilBuffer.Height = DXDevice::g_ScreenHeight;
   _depthStencilBuffer.MipLevels = 1;
   _depthStencilBuffer.ArraySize = 1;
   _depthStencilBuffer.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

   if (m_Enable4xMSAA) // Use 4X MSAA? 반드시 스왑체인 msaa값과 매치
   {
      _depthStencilBuffer.SampleDesc.Count = 4;
      _depthStencilBuffer.SampleDesc.Quality = m_4xMSAAQuality - 1;
   }
   else // No MSAA
   {
      _depthStencilBuffer.SampleDesc.Count = 1;
      _depthStencilBuffer.SampleDesc.Quality = 0;
   }

   _depthStencilBuffer.Usage = D3D11_USAGE_DEFAULT;
   _depthStencilBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
   _depthStencilBuffer.CPUAccessFlags = 0;
   _depthStencilBuffer.MiscFlags = 0;

   HR(m_pDevice->CreateTexture2D(&_depthStencilBuffer, 0, &m_pDepthStencilBuffer));

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

   HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &DXRenderState::g_DepthStencilState));

   // 깊이 스텐실 뷰의 구조체를 초기화
   D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
   ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

   // 깊이 스텐실 뷰 구조체를 설정
   depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
   depthStencilViewDesc.Texture2D.MipSlice = 0;

   HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView));

   // 렌더타겟과 뎁스/스텐실 뷰 묶음
   m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

   // 뷰포트 재설정
   m_ScreenViewport.TopLeftX = 0;
   m_ScreenViewport.TopLeftY = 0;
   m_ScreenViewport.Width = static_cast<float>(DXDevice::g_ScreenWidth);
   m_ScreenViewport.Height = static_cast<float>(DXDevice::g_ScreenHeight);
   m_ScreenViewport.MinDepth = 0.0f;
   m_ScreenViewport.MaxDepth = 1.0f;

   m_pDeviceContext->RSSetViewports(1, &m_ScreenViewport);
}

void Renderer::ShowDebugInformation()
{

}

void Renderer::DrawScreenQuad()
{
   m_pDeviceContext->IASetInputLayout(InputLayout::PosNorTex);
   m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   UINT stride = sizeof(PosNorTex);
   UINT offset = 0;

   Matrix identity = XMMatrixIdentity();

   ID3DX11EffectTechnique* texOnlyTech = StaticEffect::BasicFX->Light0TexTech;
   D3DX11_TECHNIQUE_DESC techDesc;

   texOnlyTech->GetDesc(&techDesc);
   for (UINT p = 0; p < techDesc.Passes; ++p)
   {
      m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pScreenQuadVB, &stride, &offset);
      m_pDeviceContext->IASetIndexBuffer(m_pScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);

      StaticEffect::BasicFX->SetWorld(identity);
      StaticEffect::BasicFX->SetWorldInvTranspose(identity);
      StaticEffect::BasicFX->SetWorldViewProj(identity);
      StaticEffect::BasicFX->SetTexTransform(identity);
      StaticEffect::BasicFX->SetDiffuseMap(m_pBlurFilter->GetBlurredOutput());

      texOnlyTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
      m_pDeviceContext->DrawIndexed(6, 0, 0);
   }
}

void Renderer::BuildOffScreenView()
{
   // We call this function everytime the window is resized so that the render target is a quarter
   // the client area dimensions.  So Release the previous views before we create new ones.
   ReleaseCOM(m_pOffScreenSRV);
   ReleaseCOM(m_pOffScreenRTV);
   ReleaseCOM(m_pOffScreenUAV);

   D3D11_TEXTURE2D_DESC texDesc;

   texDesc.Width = DXDevice::g_ScreenWidth;
   texDesc.Height = DXDevice::g_ScreenHeight;
   texDesc.MipLevels = 1;
   texDesc.ArraySize = 1;
   texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   texDesc.SampleDesc.Count = 1;
   texDesc.SampleDesc.Quality = 0;
   texDesc.Usage = D3D11_USAGE_DEFAULT;
   texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
   texDesc.CPUAccessFlags = 0;
   texDesc.MiscFlags = 0;

   ID3D11Texture2D* offscreenTex = 0;
   HR(m_pDevice->CreateTexture2D(&texDesc, 0, &offscreenTex));

   // Null description means to create a view to all mipmap levels using 
   // the format the texture was created with.
   HR(m_pDevice->CreateShaderResourceView(offscreenTex, 0, &m_pOffScreenSRV));
   HR(m_pDevice->CreateRenderTargetView(offscreenTex, 0, &m_pOffScreenRTV));
   HR(m_pDevice->CreateUnorderedAccessView(offscreenTex, 0, &m_pOffScreenUAV));

   // View saves a reference to the texture so we can release our reference.
   ReleaseCOM(offscreenTex);
}

void Renderer::BuildScreenQuadGeometryBuffers()
{
   GeometryGenerator::MeshData _quad;

   GeometryGenerator geoGen;
   geoGen.CreateFullscreenQuad(_quad);

   //
   // Extract the vertex elements we are interested in and pack the
   // vertices of all the meshes into one vertex buffer.
   //

   vector<PosNorTex> vertices(_quad.Vertices.size());

   for (UINT i = 0; i < _quad.Vertices.size(); ++i)
   {
      vertices[i].Pos = _quad.Vertices[i].Position;
      vertices[i].Normal = _quad.Vertices[i].Normal;
      vertices[i].Tex = _quad.Vertices[i].TexC;
   }

   D3D11_BUFFER_DESC vbd;
   vbd.Usage = D3D11_USAGE_IMMUTABLE;
   vbd.ByteWidth = sizeof(PosNorTex) * _quad.Vertices.size();
   vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vbd.CPUAccessFlags = 0;
   vbd.MiscFlags = 0;
   D3D11_SUBRESOURCE_DATA vinitData;
   vinitData.pSysMem = &vertices[0];
   HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &m_pScreenQuadVB));

   //
   // Pack the indices of all the meshes into one index buffer.
   //

   D3D11_BUFFER_DESC ibd;
   ibd.Usage = D3D11_USAGE_IMMUTABLE;
   ibd.ByteWidth = sizeof(UINT) * _quad.Indices.size();
   ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
   ibd.CPUAccessFlags = 0;
   ibd.MiscFlags = 0;
   D3D11_SUBRESOURCE_DATA iinitData;
   iinitData.pSysMem = &_quad.Indices[0];
   HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pScreenQuadIB));
}

void Renderer::BeginBlur()
{
   m_pRenderTargetViewForBlur = m_pOffScreenRTV;
   m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetViewForBlur, m_pDepthStencilView);

   // RenderTargetView 초기화
   m_pDeviceContext->ClearRenderTargetView(m_pOffScreenRTV, reinterpret_cast<const float*>(&Colors::DarkSlateGray));

   // DepthStencil 초기화
   m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::EndBlur()
{
   m_pRenderTargetViewForBlur = m_pRenderTargetView;
   m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetViewForBlur, m_pDepthStencilView);

   m_pBlurFilter->BlurInPlace(m_pDeviceContext, m_pOffScreenSRV, m_pOffScreenUAV, 4);

   // 후면 버퍼 초기화
   m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&Colors::DarkSlateGray));

   m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

   DrawScreenQuad();
}

// template <typename T>
// void Renderer::AddToObjectList(T* object)
// {
// 	IObjectBase* _tempObject = dynamic_cast<IObjectBase*>(object);
// 	m_ObjectList.push_back(_tempObject);
// }
