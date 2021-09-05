#include "Camera.h"
#include "Effect.h"
#include "ParsingData.h"
#include "ASEParser.h"
#include "StructAndConst.h"
#include "DXRenderState.h"
#include "Navigation.h"

Navigation::Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
   : IDXObjectBase(pDevice, pDeviceContext),
   m_IsInTriangle(false), m_IndexCount(), m_NaviMeshData(),
   m_pNowTriangle()
{
}

Navigation::~Navigation()
{
}

void Navigation::Initialize(ASEParser* pASEParserData)
{
   for (Mesh* _meshDataIter : pASEParserData->m_MeshData_v) // �׺�޽� �ҷ����� �־��ش�
   {
      m_NaviMeshData = _meshDataIter;
   }

   if (m_NaviMeshData->m_OptimizedVertices_v.size() <= 0)
   {
      return;
   }

   UINT vcount = 0;
   UINT tcount = 0;

   // �׺��� ���ؽ� ���� �־��ش�
   vcount = m_NaviMeshData->m_OptimizedVertices_v.size();

   vector<PosColor> vertices(vcount);
   for (UINT i = 0; i < vcount; i++)
   {      
      vertices[i].Pos.x = m_NaviMeshData->m_OptimizedVertices_v[i]->m_Pos.x;
      m_NaviMeshData->m_OptimizedVertices_v[i]->m_Pos.y = m_NaviMeshData->m_OptimizedVertices_v[i]->m_Pos.y + 0.01f;
      vertices[i].Pos.y = m_NaviMeshData->m_OptimizedVertices_v[i]->m_Pos.y;
      vertices[i].Pos.z = m_NaviMeshData->m_OptimizedVertices_v[i]->m_Pos.z;

      vertices[i].Color = Vector4((const float*)&Colors::Blue);
   }

   D3D11_BUFFER_DESC vbd;
   vbd.Usage = D3D11_USAGE_IMMUTABLE;
   vbd.ByteWidth = sizeof(PosColor) * vcount;
   vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vbd.CPUAccessFlags = 0;
   vbd.MiscFlags = 0;
   vbd.StructureByteStride = 0;
   D3D11_SUBRESOURCE_DATA vinitData;
   vinitData.pSysMem = &vertices[0];
   HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &m_pVB));

   //////////////////// index

   tcount = m_NaviMeshData->m_MeshNumFace;
   m_IndexCount = 3 * tcount;
   vector<UINT> indices(m_IndexCount);
   for (UINT i = 0; i < tcount; ++i)
   {
      indices[i * 3 + 0] = m_NaviMeshData->m_OptimizedIndices[i].Index[0];
      indices[i * 3 + 1] = m_NaviMeshData->m_OptimizedIndices[i].Index[2];
      indices[i * 3 + 2] = m_NaviMeshData->m_OptimizedIndices[i].Index[1];

      // �ﰢ���� ����� ����
      Triangle* _triangle = new Triangle();
      _triangle->aIdx = m_NaviMeshData->m_OptimizedIndices[i].Index[0];
      _triangle->bIdx = m_NaviMeshData->m_OptimizedIndices[i].Index[2];
      _triangle->cIdx = m_NaviMeshData->m_OptimizedIndices[i].Index[1];
      _triangle->index = i;
      m_Triangles_v.push_back(_triangle);
   }

   // Pack the indices of all the meshes into one index buffer.
   D3D11_BUFFER_DESC ibd;
   ibd.Usage = D3D11_USAGE_IMMUTABLE;
   ibd.ByteWidth = sizeof(UINT) * m_IndexCount;
   ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
   ibd.CPUAccessFlags = 0;
   ibd.MiscFlags = 0;
   ibd.StructureByteStride = 0;
   D3D11_SUBRESOURCE_DATA iinitData;
   iinitData.pSysMem = &indices[0];
   HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}

void Navigation::Update(Camera* pCamera, float dTime)
{
   m_World = XMMatrixIdentity();
   m_View = pCamera->View();
   m_Proj = pCamera->Proj();

   m_pNowTriangle = CheckTriangleOnNaviMesh3(m_PlayerPosition);
}

void Navigation::Render()
{
   /// NaviMesh ////////////////////////////////////////////////////////
   m_pDeviceContext->IASetInputLayout(InputLayout::PosColor);
   m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // �ε������ۿ� ���ؽ����� ����
   UINT stride = sizeof(PosColor);
   UINT offset = 0;
   m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
   m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
   // ����������Ʈ
   m_pDeviceContext->RSSetState(DXRenderState::g_WireFrame);

   // WVP TM���� ����
   Matrix worldViewProj = m_World * m_View * m_Proj;
   StaticEffect::ColorFX->SetWorldViewProj(worldViewProj);
   m_pTech = StaticEffect::ColorFX->LightTech;

   D3DX11_TECHNIQUE_DESC techDesc;
   m_pTech->GetDesc(&techDesc);
   for (UINT p = 0; p < techDesc.Passes; ++p)
   {
      m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
      // �ε����� �׸���
      m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
   }

   /*
   ID3D11Buffer* VB;
   ID3D11Buffer* IB;

   int index = CheckTriangleNumberOnNaviMesh(m_PlayerPosition);
   if (index != -1)
   {
      VB = m_Triangles_v[index]->pVBTriangle;
      IB = m_Triangles_v[index]->pIBTriangle;

      m_pDeviceContext->IASetVertexBuffers(0, 1, &VB, &_stride, &_offset);
      m_pDeviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

      m_pDeviceContext->RSSetState(DXRenderState::g_WireFrame);

      D3DX11_TECHNIQUE_DESC _techDesc;
      m_pTech->GetDesc(&_techDesc);

      for (UINT p = 0; p < _techDesc.Passes; ++p)
      {
         m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);

         m_pDeviceContext->DrawIndexed(3, 0, 0);
      }
   }
   */

   /// triangle ////////////////////////////////////////////////////////////////
   if (m_pNowTriangle != nullptr)
   {
      CreateVBIBOfTriangle(m_pNowTriangle);
      //m_pDeviceContext->IASetInputLayout(InputLayout::PosColor);
      //m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      UINT _stride = sizeof(PosColor);
      UINT _offset = 0;
      m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pNowTriangle->pVBTriangle, &_stride, &_offset);
      m_pDeviceContext->IASetIndexBuffer(m_pNowTriangle->pIBTriangle, DXGI_FORMAT_R32_UINT, 0);
      // ����������Ʈ
      m_pDeviceContext->RSSetState(DXRenderState::g_SolidFrame);

      // WVP TM���� ����
      Matrix _worldViewProj = m_World * m_View * m_Proj;
      StaticEffect::ColorFX->SetWorldViewProj(_worldViewProj);
      m_pTech = StaticEffect::ColorFX->LightTech;  

      D3DX11_TECHNIQUE_DESC _techDesc;
      m_pTech->GetDesc(&_techDesc);
      for (UINT p = 0; p < _techDesc.Passes; ++p)
      {
         m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);

         m_pDeviceContext->DrawIndexed(3, 0, 0);
      }
   }
}

Vector3 Navigation::PlayerPositionUpdate(Vector3 pos)
{
   return m_PlayerPosition = pos;
}

int Navigation::CheckTriangleNumberOnNaviMesh(Vector3 pos)
{
   float dotA = 0.0f; // a������ dot
   float dotB = 0.0f; // b������ dot
   float dotC = 0.0f; // c������ dot

   //for (Triangle _iter : m_Triangles_v)
   for (int i = 0; i < m_Triangles_v.size(); i++)
   {
      Vector3 crossAB = (m_Triangles_v[i]->b - m_Triangles_v[i]->a).Cross(pos - m_Triangles_v[i]->a);
      Vector3 crossAC = (pos - m_Triangles_v[i]->a).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->a);
      dotA = crossAB.Dot(crossAC);

      Vector3 crossBA = (m_Triangles_v[i]->a - m_Triangles_v[i]->b).Cross(pos - m_Triangles_v[i]->b);
      Vector3 crossBC = (pos - m_Triangles_v[i]->b).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->b);
      dotB = crossBA.Dot(crossBC);

      Vector3 crossCB = (m_Triangles_v[i]->b - m_Triangles_v[i]->c).Cross(pos - m_Triangles_v[i]->c);
      Vector3 crossCA = (pos - m_Triangles_v[i]->c).Cross(m_Triangles_v[i]->a - m_Triangles_v[i]->c);
      dotC = crossCB.Dot(crossCA);

      if (dotA > 0.0f && dotB > 0.0f && dotC > 0.0f) // ��� ����� �ﰢ�� �ȿ� �ִ°�
      {
         return i; // i��° �ﰢ�� �ȿ� ���̰� ����
      }
   }

   return (-1); // ���� ������ ó��
}

Triangle* Navigation::CheckTriangleOnNaviMesh(Vector3 pos)
{
   float dotA = 0.0f; // a������ dot
   float dotB = 0.0f; // b������ dot
   float dotC = 0.0f; // c������ dot

   //for (Triangle _iter : m_Triangles_v)
   for (int i = 0; i < m_Triangles_v.size(); i++)
   {
      Vector3 crossAB = (m_Triangles_v[i]->b - m_Triangles_v[i]->a).Cross(pos - m_Triangles_v[i]->a);
      Vector3 crossAC = (pos - m_Triangles_v[i]->a).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->a);
      dotA = crossAB.Dot(crossAC);

      Vector3 crossBA = (m_Triangles_v[i]->a - m_Triangles_v[i]->b).Cross(pos - m_Triangles_v[i]->b);
      Vector3 crossBC = (pos - m_Triangles_v[i]->b).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->b);
      dotB = crossBA.Dot(crossBC);

      Vector3 crossCB = (m_Triangles_v[i]->b - m_Triangles_v[i]->c).Cross(pos - m_Triangles_v[i]->c);
      Vector3 crossCA = (pos - m_Triangles_v[i]->c).Cross(m_Triangles_v[i]->a - m_Triangles_v[i]->c);
      dotC = crossCB.Dot(crossCA);

      if (dotA > 0.0f && dotB > 0.0f && dotC > 0.0f) // ��� ����� �ﰢ�� �ȿ� �ִ°�
      {
         return m_Triangles_v[i]; // i��° �ﰢ�� �ȿ� ���̰� ����
      }
   }

   return nullptr; // ���� ������ ó��
}

Triangle* Navigation::CheckTriangleOnNaviMesh2(Vector3 pos)
{
   float dotA = 0.0f; // a������ dot
   float dotB = 0.0f; // b������ dot
   float dotC = 0.0f; // c������ dot

   for (int i = 0; i < m_Triangles_v.size(); i++)
   {
      // �ﰢ���� �� ������
      Vector3 nowTrngA = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->aIdx]->m_Pos;
      Vector3 nowTrngB = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->bIdx]->m_Pos;
      Vector3 nowTrngC = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->cIdx]->m_Pos;

      Vector3 crossAB = (nowTrngB - nowTrngA).Cross(pos - nowTrngA);
      Vector3 crossAC = (pos - nowTrngA).Cross(nowTrngC - nowTrngA);
      dotA = crossAB.Dot(crossAC);

      Vector3 crossBA = (nowTrngA - nowTrngB).Cross(pos - nowTrngB);
      Vector3 crossBC = (pos - nowTrngB).Cross(nowTrngC - nowTrngB);
      dotB = crossBA.Dot(crossBC);

      Vector3 crossCB = (nowTrngB - nowTrngC).Cross(pos - nowTrngC);
      Vector3 crossCA = (pos - nowTrngC).Cross(nowTrngA - nowTrngC);
      dotC = crossCB.Dot(crossCA);

      if (dotA > 0.0f && dotB > 0.0f && dotC > 0.0f) // ��� ����� �ﰢ�� �ȿ� �ִ°�
      {
         m_Triangles_v[i]->a = nowTrngA;
         m_Triangles_v[i]->b = nowTrngB;
         m_Triangles_v[i]->c = nowTrngC;

         m_IsInTriangle = true;

         return m_Triangles_v[i]; // i��° �ﰢ�� �ȿ� ���̰� ����
      }
   }

   m_IsInTriangle = false; // �ﰢ���� �� ���Ҵµ� ����... = ���̴�

   return nullptr; // ���� ������ ó��
}

void Navigation::CreateVBIBOfTriangle(Triangle* triangle)
{
   /////////////////////////// triangle vertex ///////////////////
   PosColor _vertices[3];
   _vertices[0].Pos = triangle->a;
   _vertices[1].Pos = triangle->b;
   _vertices[2].Pos = triangle->c;
   _vertices[0].Color = Vector4((const float*)&Colors::DarkGray);
   _vertices[1].Color = Vector4((const float*)&Colors::DarkGray);
   _vertices[2].Color = Vector4((const float*)&Colors::DarkGray);

   D3D11_BUFFER_DESC _vbdTriangle;
   _vbdTriangle.Usage = D3D11_USAGE_IMMUTABLE;
   _vbdTriangle.ByteWidth = sizeof(PosColor) * 3;
   _vbdTriangle.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   _vbdTriangle.CPUAccessFlags = 0;
   _vbdTriangle.MiscFlags = 0;
   _vbdTriangle.StructureByteStride = 0;
   D3D11_SUBRESOURCE_DATA vinitDataTriangle;
   vinitDataTriangle.pSysMem = &_vertices[0];
   HR(m_pDevice->CreateBuffer(&_vbdTriangle, &vinitDataTriangle, &triangle->pVBTriangle));

   /////////////////////////////// triangle index /////////////////
   UINT _indices[3];
   _indices[0] = 0;
   _indices[1] = 1;
   _indices[2] = 2; /// �̰� ���ٷ��ؼ� �ݴ�� �׷������־���...

   D3D11_BUFFER_DESC _ibdTriangle;
   _ibdTriangle.Usage = D3D11_USAGE_IMMUTABLE;
   _ibdTriangle.ByteWidth = sizeof(UINT) * 3;
   _ibdTriangle.BindFlags = D3D11_BIND_INDEX_BUFFER;
   _ibdTriangle.CPUAccessFlags = 0;
   _ibdTriangle.MiscFlags = 0;
   _vbdTriangle.StructureByteStride = 0;
   D3D11_SUBRESOURCE_DATA iinitDataTriangle;
   iinitDataTriangle.pSysMem = &_indices[0];
   HR(m_pDevice->CreateBuffer(&_ibdTriangle, &iinitDataTriangle, &triangle->pIBTriangle));
}

/// �ﰢ�� �ȿ� ���� �ִ��� Ȯ���ϴ� ���(dot, cross�̿�)
// �� a, b, c�� �̷��� �ﰢ���� �� p
// (ab x ap) dot (ap x ac) �� ���� ��� ���������� ����� p�� �ﰢ�� �ȿ��ִ�
Triangle* Navigation::CheckTriangleOnNaviMesh3(Vector3 pos)
{
   float dotA = 0.0f; // a������ dot
   float dotB = 0.0f; // b������ dot
   float dotC = 0.0f; // c������ dot    

   for (int i = 0; i < m_Triangles_v.size(); i++)
   {
      // �ﰢ���� �޽������Ϳ��� �޾ƿ´� 
      m_Triangles_v[i]->a = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->aIdx]->m_Pos;
      m_Triangles_v[i]->b = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->bIdx]->m_Pos;
      m_Triangles_v[i]->c = m_NaviMeshData->m_OptimizedVertices_v[m_Triangles_v[i]->cIdx]->m_Pos;

      // �Ű������� ������ ��ġ���� �ﰢ������ ������ ��ġ���� ���
      Vector3 _posOnTrng = PositionInTriangle(pos, m_Triangles_v[i]);
       
      Vector3 crossAB = (m_Triangles_v[i]->b - m_Triangles_v[i]->a).Cross(_posOnTrng - m_Triangles_v[i]->a);
      Vector3 crossAC =          (_posOnTrng - m_Triangles_v[i]->a).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->a);
      dotA = crossAB.Dot(crossAC);

      Vector3 crossBA = (m_Triangles_v[i]->a - m_Triangles_v[i]->b).Cross(_posOnTrng - m_Triangles_v[i]->b);
      Vector3 crossBC =          (_posOnTrng - m_Triangles_v[i]->b).Cross(m_Triangles_v[i]->c - m_Triangles_v[i]->b);
      dotB = crossBA.Dot(crossBC);

      Vector3 crossCB = (m_Triangles_v[i]->b - m_Triangles_v[i]->c).Cross(_posOnTrng - m_Triangles_v[i]->c);
      Vector3 crossCA =          (_posOnTrng - m_Triangles_v[i]->c).Cross(m_Triangles_v[i]->a - m_Triangles_v[i]->c);
      dotC = crossCB.Dot(crossCA);

      if (dotA > 0.0f && dotB > 0.0f && dotC > 0.0f) 
      {
         m_IsInTriangle = true; // ��� ����� �ﰢ�� �ȿ� �ִ°�

         return m_Triangles_v[i]; // i��° �ﰢ�� �ȿ� ���̰� ����
      }
   }

   m_IsInTriangle = false; // �ﰢ���� �� ���Ҵµ� ���� = ���̴�

   return nullptr; 
}

Vector3 Navigation::VectorToKeepInTriangle(Vector3 pos)
{
   Vector3 _vec; // S
   pos; // pOrigin
   Vector3 vDir = {0.0f, -1.0f, 0.0f}; // vDir
   Vector3 N = NormalVectorOfTriangle(m_pNowTriangle); // N
   N.Normalize();

   Vector3 P0 = m_pNowTriangle->a; // P0
   Vector3 vResult = P0 - pos;
   float fResult = vResult.Dot(N);
   float f_t = fResult / (vDir.Dot(N));

   _vec = pos + f_t * vDir;

   return _vec;
}

Vector3 Navigation::PositionInTriangle(Vector3 pos, Triangle* triangle)
{
   // ������ �����Ŀ��� : posOnTrng = pos + d*posDir // d�� ���Ѵ�
   // ����� �����Ŀ��� : trngNor dot (p0 - posOnTrng) = 0 
   // posOnTrng�� �����Ͽ� d�� ����
   Vector3 posInTrng; // �ﰢ�� ���� �� : ��� ��

   Vector3 posDir = { 0.0f, -1.0f, 0.0f }; // pos�� ���� ���� -Y�� 
   Vector3 trngNor = NormalVectorOfTriangle(triangle); // �ﰢ���� �븻����
   Vector3 p0 = triangle->a; // �ﰢ������ �� ��

   // ������
   Vector3 vecOnTrng = p0 - pos;
   float distToTrng = (vecOnTrng.Dot(trngNor)) / (posDir.Dot(trngNor));
   posInTrng = pos + distToTrng * posDir;

   return posInTrng;
}

Vector3 Navigation::NormalVectorOfTriangle(Triangle* triangle)
{
   // �ﰢ���� �븻 ����ϱ� 
   Vector3 vecAB = triangle->b - triangle->a;
   Vector3 vecAC = triangle->c - triangle->a;
   // �ﰢ�� �� �κ��͸� �����ϸ� �븻�� ���´�
   Vector3 _normal = vecAC.Cross(vecAB);   
   _normal.Normalize();

   return _normal;
}
