#include "Sky.h"
#include "DDSTextureLoader.h"
#include "GeometryGenerator.h"
#include "Camera.h"
#include "StructAndConst.h"
#include "Effect.h"


Sky::Sky(ID3D11Device* pDevice, const wstring& fileName, float skySphereRadius)
{
   ID3D11Resource* texResource = nullptr;
   HR(DirectX::CreateDDSTextureFromFile(pDevice, fileName.c_str(), &texResource, &m_pCubeMapSRV));
   ReleaseCOM(texResource); // view saves reference

   GeometryGenerator::MeshData sphere;
   GeometryGenerator geoGen;
   geoGen.CreateSphere(skySphereRadius, 30, 30, sphere);

   std::vector<XMFLOAT3> vertices(sphere.Vertices.size());

   for (size_t i = 0; i < sphere.Vertices.size(); ++i)
   {
      vertices[i] = sphere.Vertices[i].Position;
   }

   D3D11_BUFFER_DESC vbd;
   vbd.Usage = D3D11_USAGE_IMMUTABLE;
   vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
   vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vbd.CPUAccessFlags = 0;
   vbd.MiscFlags = 0;
   vbd.StructureByteStride = 0;

   D3D11_SUBRESOURCE_DATA vinitData;
   vinitData.pSysMem = &vertices[0];

   HR(pDevice->CreateBuffer(&vbd, &vinitData, &m_pVB));


   m_IndexCount = sphere.Indices.size();

   D3D11_BUFFER_DESC ibd;
   ibd.Usage = D3D11_USAGE_IMMUTABLE;
   ibd.ByteWidth = sizeof(USHORT) * m_IndexCount;
   ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
   ibd.CPUAccessFlags = 0;
   ibd.StructureByteStride = 0;
   ibd.MiscFlags = 0;

   std::vector<USHORT> indices16;
   indices16.assign(sphere.Indices.begin(), sphere.Indices.end());

   D3D11_SUBRESOURCE_DATA iinitData;
   iinitData.pSysMem = &indices16[0];

   HR(pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}

Sky::~Sky()
{
   ReleaseCOM(m_pVB);
   ReleaseCOM(m_pIB);
   ReleaseCOM(m_pCubeMapSRV);
}

ID3D11ShaderResourceView* Sky::CubeMapSRV()
{
   return m_pCubeMapSRV;
}

void Sky::Draw(ID3D11DeviceContext* pDC, const Camera& camera)
{
   // center Sky about eye in world space
   Vector3 eyePos = camera.GetPosition();
   Matrix T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);


   Matrix WVP = XMMatrixMultiply(T, camera.ViewProj());

   StaticEffect::SkyFX->SetWorldViewProj(WVP);
   StaticEffect::SkyFX->SetCubeMap(m_pCubeMapSRV);


   UINT stride = sizeof(Vector3);
   UINT offset = 0;
   pDC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
   pDC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);
   pDC->IASetInputLayout(InputLayout::Pos);
   pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   D3DX11_TECHNIQUE_DESC techDesc;
   StaticEffect::SkyFX->SkyTech->GetDesc(&techDesc);

   for (UINT p = 0; p < techDesc.Passes; ++p)
   {
      ID3DX11EffectPass* pass = StaticEffect::SkyFX->SkyTech->GetPassByIndex(p);

      pass->Apply(0, pDC);

      pDC->DrawIndexed(m_IndexCount, 0, 0);
   }
}