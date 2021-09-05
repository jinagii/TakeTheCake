#include "GeometryHelper.h"
#include "DXDevice.h"
#include "Effect.h"
#include "DXRenderState.h"
#include "ImageObject.h"

ImageObject::ImageObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
   : IDXObjectBase(pDevice, pDeviceContext), m_VertexCount(), m_IndexCount(), m_pImage(nullptr),
   m_ImageWidth(),m_ImageHeight(), m_PosX(), m_PosY(), m_InitialScrnWidth(), m_InitialScrnHeight()
{
   
}

ImageObject::~ImageObject()
{
   ReleaseCOM(m_pImage);
}

void ImageObject::Initialize(int x, int y, int width, int height)
{
   m_PosX = x + DXDevice::g_WndStartX;
   m_PosY = y + DXDevice::g_WndStartY;
   m_ImageWidth = width;
   m_ImageHeight = height;
   m_InitialScrnWidth = DXDevice::g_ScreenWidth;
   m_InitialScrnHeight = DXDevice::g_ScreenHeight;

   m_pDebugCircle = nullptr;
}

void ImageObject::Update(Camera* pCamera, float dTime)
{
   m_Proj = XMMatrixOrthographicLH(DXDevice::g_ScreenWidth, DXDevice::g_ScreenHeight, 0.1f, 1000.0f);
   //m_Proj = XMMatrixOrthographicLH(m_ImageWidth, m_ImageHeight, 0.1f, 1000.0f);

   float _left;
   float _top;
   float _right;
   float _bottom;

   PosTex* _vertices = new PosTex[m_VertexCount];
   D3D11_MAPPED_SUBRESOURCE mappedResource;
   PosTex* _pVertices;
   HRESULT result;

   float _ratioWidth = (float)DXDevice::g_ScreenWidth/(float)m_InitialScrnWidth;
   float _ratioHeight = (float)DXDevice::g_ScreenHeight/ (float)m_InitialScrnHeight;

   float _imageWidth = m_ImageWidth * _ratioWidth;
   float _imageHeight = m_ImageHeight * _ratioHeight;

   _left = (float)(m_PosX - (float)(DXDevice::g_ScreenWidth / 2));
   _top = (float)((float)(DXDevice::g_ScreenHeight / 2) - m_PosY);
   _right = _left + (float)_imageWidth;
   _bottom = _top - (float)_imageHeight;

   // 배열에 데이터 로드 // 삼각형 두개
   _vertices[0].Pos = Vector3(_left, _bottom, 0.0f);
   _vertices[0].Tex = Vector2(0.0f, 1.0f);
   _vertices[1].Pos = Vector3(_right, _bottom, 0.0f);
   _vertices[1].Tex = Vector2(1.0f, 1.0f);
   _vertices[2].Pos = Vector3(_left, _top, 0.0f);
   _vertices[2].Tex = Vector2(0.0f, 0.0f);

   _vertices[3].Pos = Vector3(_right, _bottom, 0.0f);
   _vertices[3].Tex = Vector2(1.0f, 1.0f);
   _vertices[4].Pos = Vector3(_right, _top, 0.0f);
   _vertices[4].Tex = Vector2(1.0f, 0.0f);
   _vertices[5].Pos = Vector3(_left, _top, 0.0f);
   _vertices[5].Tex = Vector2(0.0f, 0.0f);

   // 버텍스 버퍼를 쓸 수 있도록 잠금
   result = m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
   // 버텍스에 데이터를 가리키는 포인터 얻는다
   _pVertices = (PosTex*)mappedResource.pData;
   // 데이터를 버텍스 버퍼에 복사
   memcpy(_pVertices, (void*)_vertices, sizeof(PosTex) * m_VertexCount);
   
   // 버텍스 버퍼 잠금 해제
   m_pDeviceContext->Unmap(m_pVB, 0);

   delete[] _vertices;
   _vertices = nullptr;
}

void ImageObject::Render()
{
   m_pDeviceContext->OMSetDepthStencilState(DXRenderState::g_OffDepthStencilState,1);

   m_pDeviceContext->IASetInputLayout(InputLayout::PosTex);
   m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // 정점 버퍼의 단위와 오프셋을 설정합니다.
   UINT stride = sizeof(PosTex);
   UINT offset = 0;
   m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
   m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

   // Set constants
   XMMATRIX world = XMMatrixIdentity();
   XMMATRIX view = XMMatrixIdentity();
   XMMATRIX proj = m_Proj;
   XMMATRIX worldViewProj = world * view * proj;

   StaticEffect::BitmapFX->SetWorldViewProj(worldViewProj);

   // Render State
   m_pDeviceContext->RSSetState(DXRenderState::g_SolidFrame);

   //ID3DX11EffectTechnique* activeTech = StaticEffect::BitmapFX->LightTech;
   m_pTech = StaticEffect::BitmapFX->LightTech;

   D3DX11_TECHNIQUE_DESC techDesc;
   m_pTech->GetDesc(&techDesc);
   for (UINT p = 0; p < techDesc.Passes; ++p)
   {
      StaticEffect::BitmapFX->SetDiffuseMap(m_pImage);

      m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
      m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
   }

   m_pDeviceContext->OMSetDepthStencilState(DXRenderState::g_DepthStencilState,1);
}

void ImageObject::LoadGeometry(DXGeometryBuffer* pGB)
{
   m_pIB = pGB->pIB;
   m_pVB = pGB->pVB;
   m_VertexCount = pGB->VertexCnt;
   m_IndexCount = pGB->IndexCnt;
}

void ImageObject::SetImageSRV(ID3D11ShaderResourceView* pSRV)
{
   m_pImage = pSRV;
}

