#pragma once
#include "DXDefine.h"
#include "IDXObjectBase.h"

class GeometryHelper;

class ImageObject : public IDXObjectBase
{
public:
   ImageObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
   virtual ~ImageObject();

   void Initialize(int x, int y, int width, int height);
   virtual void Update(Camera* pCamera, float dTime);
   virtual void Render();

   virtual void LoadGeometry(DXGeometryBuffer* pGB);

   void SetImageSRV(ID3D11ShaderResourceView* pSRV);      


private:
   UINT m_VertexCount;
   UINT m_IndexCount;

   ID3D11ShaderResourceView* m_pImage;

   int m_InitialScrnWidth;
   int m_InitialScrnHeight;


   float m_ImageWidth;
   float m_ImageHeight;

   // 좌상단 시작점
   int m_PosX;
   int m_PosY;


};

