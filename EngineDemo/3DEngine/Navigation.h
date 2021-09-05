#pragma once

#include "IDXObjectBase.h"
#include "DXDefine.h"

/// �׺���̼� �޽�
// ���� �� ��� �ﰢ�����ε� �׷� (wire frame)
// �ﰢ���� ��� ��ȯ�Ѵ�
// �׺�޽��� ���� y�࿡ ������ ������ �浹������ �Ѵ�
// return������ vector3

/// �ﰢ�� �ȿ� ���� �ִ��� Ȯ���ϴ� ���(dot, cross�̿�)
// �� a, b, c�� �̷��� �ﰢ���� �� p
// (ab x ap) dot (ap x ac) �� ���� ��� ���������� ����� p�� �ﰢ�� �ȿ��ִ�

/// ����..�̰� ���İ� �Ф�

class Camera;
class ASEParser;

struct Triangle
{
   // triangle vertex positions
   Vector3 a;
   Vector3 b;
   Vector3 c;

   UINT aIdx = 0;
   UINT bIdx = 0;
   UINT cIdx = 0;

   int index = 0;

   ID3D11Buffer* pVBTriangle = nullptr;
   ID3D11Buffer* pIBTriangle = nullptr;
};

class Navigation : public IDXObjectBase
{
public:
   Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
   virtual ~Navigation();

   virtual void Initialize(ASEParser* pASEParserData);
   virtual void Update(Camera* pCamera, float dTime);
   virtual void Render();

   void CreateVBIBOfTriangle(Triangle* triangle);
      
   // MK1
   Vector3 PlayerPositionUpdate(Vector3 pos);
   int CheckTriangleNumberOnNaviMesh(Vector3 pos);
   Triangle* CheckTriangleOnNaviMesh(Vector3 pos);

   // MK2
   Triangle* CheckTriangleOnNaviMesh2(Vector3 pos);
   Vector3 VectorToKeepInTriangle(Vector3 pos);

   // MK3
   Triangle* CheckTriangleOnNaviMesh3(Vector3 pos);
   Vector3 PositionInTriangle(Vector3 pos, Triangle* triangle);
   Vector3 NormalVectorOfTriangle(Triangle* triangle);

   bool GetIsInTriangle() const { return m_IsInTriangle; }
   void SetIsInTriangle(bool val) { m_IsInTriangle = val; }

private:
   // ó���� �ʿ��ҰŶ� �����ߴ��͵�
   //Vector3 m_NaviRay; // Y�࿡ ������ Ray
   //Vector3 m_NowPosition; // ���� ��ġ��
   //Vector3 m_LastPosition; // ���� ������ϸ� �ǵ��� ��ǥ? 
   
   // ���� ���°͵�
   Mesh* m_NaviMeshData;

   vector<Triangle*> m_Triangles_v;
   Triangle* m_pNowTriangle;   
   bool m_IsInTriangle;

   Vector3 m_PlayerPosition;

   UINT m_IndexCount;
};

