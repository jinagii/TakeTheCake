#pragma once

#include "DXDefine.h"

class Camera;

class Sky
{
public:
   Sky(ID3D11Device* pDevice, const wstring& fileName, float skySphereRadius);
   ~Sky();

   ID3D11ShaderResourceView* CubeMapSRV();

   void Draw(ID3D11DeviceContext* pDC, const Camera& camera);

private:
   Sky(const Sky& rhs);
   Sky& operator=(const Sky& rhs);

private:
   ID3D11Buffer* m_pVB;
   ID3D11Buffer* m_pIB;

   ID3D11ShaderResourceView* m_pCubeMapSRV;

   UINT m_IndexCount;
};