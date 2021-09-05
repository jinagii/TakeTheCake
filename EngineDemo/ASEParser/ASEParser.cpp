//#include "Lexer.h"
//#include "Animation.h"
#include "ASEParser.h"

using namespace SimpleMath;

ASEParser::ASEParser()
   : m_materialcount(0), m_Animation(0), m_Lexer(0), m_OneMeshData(0), m_Token(0), m_TokenString(), m_data_asciiexport(0),
   m_nowshapeline(0), m_nowshapevertex(0)
{
   m_materialcount = 0;
   m_ParsingMode = eNone;
}

ASEParser::~ASEParser()
{
   delete m_Lexer;
}

bool ASEParser::Initialize()
{
   m_Lexer = new ASELexer();

   return TRUE;
}

// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
bool ASEParser::LoadFile(LPSTR file)
{
   /// 0) ������ �ε��Ѵ�.
   if (!m_Lexer->Open(file))
   {
      TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
      return FALSE;
   }

   ParsingDivergeRecursiveALL(0);

   return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool ASEParser::TranslateToD3DFormat_scene(Scenedata* pScene)
{
   // �� ����
   *pScene = this->m_SceneData;
   return TRUE;
}

/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
bool ASEParser::VertexOptimization(Mesh* pMesh)
{
   if (pMesh->m_MeshVertices_v.size() > 0)
   {
      for (UINT _ver = 0; _ver < pMesh->m_Mesh_NumVertex; _ver++) // vertex ����
      {
         /// �ϴ� load�� vertex���� optimized vertex vector�� ����
         Vertex* _pVertex = new Vertex();

         _pVertex->m_Pos = pMesh->m_MeshVertices_v[_ver]->m_Pos;
         _pVertex->m_Indices = pMesh->m_MeshVertices_v[_ver]->m_Indices;

         /// texture ���� ����
         _pVertex->m_U = pMesh->m_MeshVertices_v[_ver]->m_U;
         _pVertex->m_V = pMesh->m_MeshVertices_v[_ver]->m_V;
         _pVertex->m_Normal = pMesh->m_MeshVertices_v[_ver]->m_Normal;

         pMesh->m_OptimizedVertices_v.push_back(_pVertex); /// ����ȭ�� vertex�� ��ƾ� ��
      }
   }
   else
   {
      return false;
   }

   /// Face�� ��� ���鼭 ���ؽ����� �븻���� ����
   if (pMesh->m_MeshFaces_v.size() > 0)
   {
      for (Face* _meshFaceIter : pMesh->m_MeshFaces_v)
      {
         for (UINT j = 0; j < 3; j++)
         {
            Vertex* _currVertex = pMesh->m_OptimizedVertices_v[_meshFaceIter->m_VertexIndex[j]];

            if (pMesh->m_MeshTVertices_v.size() > 0)
            {
               _currVertex->m_U = pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_u;
               _currVertex->m_V = pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_v;
            }

            //if (pMesh->m_IsSkinningObject == true)
            //{
            //   _currVertex->m_BW1 = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_BW1;
            //   _currVertex->m_BW2 = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_BW2;
            //   _currVertex->m_BW3 = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_BW3;
            //   _currVertex->m_WeightIndex[0] = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_WeightIndex[0];
            //   _currVertex->m_WeightIndex[1] = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_WeightIndex[1];
            //   _currVertex->m_WeightIndex[2] = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_WeightIndex[2];
            //   _currVertex->m_WeightIndex[3] = pMesh->m_MeshVertices_v[_meshFaceIter->m_VertexIndex[j]]->m_WeightIndex[3];
            //}

            //_currVertex->m_Normal = _meshFaceIter->m_VertexNormal[j];

            if (_currVertex->m_Normal != _meshFaceIter->m_VertexNormal[j] ||
               _currVertex->m_U != pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_u ||
               _currVertex->m_V != pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_v)
            {
               /// ���� ����� �־� ����� 
               Vertex* _newVertex = new Vertex();
               _newVertex->m_Pos = _currVertex->m_Pos;
               _newVertex->m_Indices = _currVertex->m_Indices;
               _newVertex->m_Normal = _meshFaceIter->m_VertexNormal[j];

               /// texture�� ������ u, v �־���
               if (pMesh->m_Mesh_NumTVertex != 0)
               {
                  _newVertex->m_U = pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_u;
                  _newVertex->m_V = pMesh->m_MeshTVertices_v[_meshFaceIter->m_TFace[j]]->m_v;
               }

               /// �׷��� index�� �߰��ؾ��Ѵ� 
               int _newVerIndex = pMesh->m_OptimizedVertices_v.size(); // ������� 1ũ�� 

               /// index �ٲ��ְ�
               _meshFaceIter->m_VertexIndex[j] = _newVerIndex;
               // vector�� �־���
               pMesh->m_OptimizedVertices_v.push_back(_newVertex);
            }
         }
      }

      /*
      for (UINT i = 0; i < pMesh->m_MeshFaces_v.size(); i++)
      {
         /// Face���� vertex index�� 3���� (3����)
         for (UINT j = 0; j < 3; j++)
         {
            // ���� vertex�� �������� �񱳸� �ǽ��ؾ���
            Vertex* _currVertex = pMesh->m_OptimizedVertices_v[pMesh->m_MeshFaces_v[i]->m_VertexIndex[j]];

            /// texture�� ������ u, v�� �����Ѵ�
            if (pMesh->m_Mesh_NumTVertex != 0)
            {
               _currVertex->m_U = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_u;
               _currVertex->m_V = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_v;
            }

            /// parser�� ���� meshvertices�� optimizedvertices�� ���ؼ� position�� ������
            /// normal, u, v �� �߿� ���� �ٸ��� ������ ���� �������Ѵ�
            //if (pMesh->m_MeshTVertices_v.size() == 0) break;

            if (_currVertex->m_Normal != pMesh->m_MeshFaces_v[i]->m_VertexNormal[j] ||
               _currVertex->m_U != pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_u ||
               _currVertex->m_V != pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_v)
            {
               /// ���� ����� �־� ����� 
               Vertex* _newVertex = new Vertex();
               _newVertex->m_Pos = _currVertex->m_Pos;
               _newVertex->m_Indices = _currVertex->m_Indices;
               _newVertex->m_Normal = pMesh->m_MeshFaces_v[i]->m_VertexNormal[j];

               /// texture�� ������ u, v �־���
               if (pMesh->m_Mesh_NumTVertex != 0)
               {
                  _newVertex->m_U = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_u;
                  _newVertex->m_V = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_v;
               }

               /// �׷��� index�� �߰��ؾ��Ѵ� 
               int _newVerIndex = pMesh->m_OptimizedVertices_v.size(); // ������� 1ũ�� 

               /// index �ٲ��ְ�
               pMesh->m_MeshFaces_v[i]->m_VertexIndex[j] = _newVerIndex;
               // vector�� �־���
               pMesh->m_OptimizedVertices_v.push_back(_newVertex);
            }

            //_nowVertex->m_Normal = pMesh->m_MeshFaces[i]->m_VertexNormal[j];
            //
            //_nowVertex->m_U = pMesh->m_Mesh_TVertices[pMesh->m_MeshFaces[i]->m_TFace[j]]->m_u;
            //_nowVertex->m_V = pMesh->m_Mesh_TVertices[pMesh->m_MeshFaces[i]->m_TFace[j]]->m_v;
         }
      }

      */
   }
   //else
   //{
   //   /// vertex���� index�鵵 optimized index vector�� ����
   //   pMesh->m_OptimizedIndices = new IndexList[pMesh->m_MeshFaces_v.size()];
   //
   //   for (UINT i = 0; i < pMesh->m_MeshFaces_v.size(); i++)
   //   {
   //      for (UINT j = 0; j < 3; j++)
   //      {
   //         pMesh->m_OptimizedIndices[i].Index[j] = pMesh->m_MeshFaces_v[i]->m_VertexIndex[j];
   //      }
   //   }
   //}

   /// vertex���� index�鵵 optimized index vector�� ����
   pMesh->m_OptimizedIndices = new IndexList[pMesh->m_MeshFaces_v.size()];

   for (UINT i = 0; i < pMesh->m_MeshFaces_v.size(); i++)
   {
      for (UINT j = 0; j < 3; j++)
      {
         pMesh->m_OptimizedIndices[i].Index[j] = pMesh->m_MeshFaces_v[i]->m_VertexIndex[j];
      }
   }

   return TRUE;
}

bool ASEParser::ConvertAll(Mesh* pMesh)
{
   // ���ؽ����� �ϴ� ��� ����
   if (pMesh->m_MeshVertices_v.size() > 0)
   {
      for (UINT i = 0; i < pMesh->m_MeshVertices_v.size(); i++)
      {
         Vertex* _pVertex = new Vertex;
         _pVertex->m_Pos = pMesh->m_MeshVertices_v[i]->m_Pos;

         pMesh->m_OptimizedVertices_v.push_back(_pVertex);
      }
   }
   else
   {
      return false;
   }

   // �������� face�� �������� �� �ε����� ã�ƾ� ��
   unsigned int _faceCount = pMesh->m_MeshFaces_v.size();
   TRACE("FaceCount : %d", _faceCount);

   for (UINT i = 0; i < pMesh->m_MeshFaces_v.size(); i++)
   {
      TRACE("FaceNum : %d / %d\n", i, _faceCount);

      for (int j = 0; j < 3; j++)
      {
         Vertex* _nowVertex = pMesh->m_OptimizedVertices_v[pMesh->m_MeshFaces_v[i]->m_VertexIndex[j]];

         // face�������� ���ؽ��� �븻�� �־��ش�.
         // (���ÿ�)
         _nowVertex->m_Normal.x = pMesh->m_MeshFaces_v[i]->m_VertexNormal[j].x;
         _nowVertex->m_Normal.y = pMesh->m_MeshFaces_v[i]->m_VertexNormal[j].y;
         _nowVertex->m_Normal.z = pMesh->m_MeshFaces_v[i]->m_VertexNormal[j].z;

         _nowVertex->m_U = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_u;
         _nowVertex->m_V = pMesh->m_MeshTVertices_v[pMesh->m_MeshFaces_v[i]->m_TFace[j]]->m_v;
      }
   }

   // �ε����� �׳� ����
   pMesh->m_OptimizedIndices = new IndexList[pMesh->m_MeshFaces_v.size()];

   for (UINT i = 0; i < pMesh->m_MeshFaces_v.size(); i++)
   {
      for (int j = 0; j < 3; j++)
      {
         pMesh->m_OptimizedIndices[i].Index[j] = pMesh->m_MeshFaces_v[i]->m_VertexIndex[j];
      }
   }

   return TRUE;
}

bool ASEParser::SetLocalTM(Mesh* pMesh)
{
   // Parsing�� ����node�� 
   Matrix _worldNodeTM = {
           pMesh->m_TMRow0.x, pMesh->m_TMRow0.y, pMesh->m_TMRow0.z, 0.0f,
           pMesh->m_TMRow1.x, pMesh->m_TMRow1.y, pMesh->m_TMRow1.z, 0.0f,
           pMesh->m_TMRow2.x, pMesh->m_TMRow2.y, pMesh->m_TMRow2.z, 0.0f,
           pMesh->m_TMRow3.x, pMesh->m_TMRow3.y, pMesh->m_TMRow3.z, 1.0f };

   // ���� node�� ������� ���ؽ����� �����ָ� ������ǥ(����)���� ������
   for (UINT i = 0; i < pMesh->m_OptimizedVertices_v.size(); i++)
   {
      Vector3 _nowPos = { pMesh->m_OptimizedVertices_v[i]->m_Pos.x,
                          pMesh->m_OptimizedVertices_v[i]->m_Pos.y,
                          pMesh->m_OptimizedVertices_v[i]->m_Pos.z }; // ������ 1: ����� �ϴ���...

      pMesh->m_OptimizedVertices_v[i]->m_Pos = Vector3::Transform(_nowPos, _worldNodeTM.Invert());
   }

   /// �θ��� �����ϸ�
   if (pMesh->m_IsParentExist == true)
   {
      for (UINT i = 0; i < m_MeshData_v.size(); i++)
      {
         // ��� �̸� ������ �н�
         if (pMesh->m_NodeName == m_MeshData_v[i]->m_NodeName) continue;

         // ���� �޽��� �θ����̸��� �θ����� ����̸��� ������
         if (pMesh->m_ParentNodeName == m_MeshData_v[i]->m_NodeName)
         {
            Matrix _parWorldNodeTM = {
                m_MeshData_v[i]->m_TMRow0.x, m_MeshData_v[i]->m_TMRow0.y, m_MeshData_v[i]->m_TMRow0.z, 0.0f,
                m_MeshData_v[i]->m_TMRow1.x, m_MeshData_v[i]->m_TMRow1.y, m_MeshData_v[i]->m_TMRow1.z, 0.0f,
                m_MeshData_v[i]->m_TMRow2.x, m_MeshData_v[i]->m_TMRow2.y, m_MeshData_v[i]->m_TMRow2.z, 0.0f,
                m_MeshData_v[i]->m_TMRow3.x, m_MeshData_v[i]->m_TMRow3.y, m_MeshData_v[i]->m_TMRow3.z, 1.0f };

            // LocalTM = WorldNodeTM * ParentWorldNodeTM^-1
            pMesh->m_LocalTM = _worldNodeTM * _parWorldNodeTM.Invert();

            // �θ�޽� �����͸� �־��ش�
            pMesh->m_pParentMesh = m_MeshData_v[i];
         }
      }
   }
   else // �θ��� ������
   {
      pMesh->m_LocalTM = _worldNodeTM; // ���� ��尡 �� ���� ���
   }

   return true;
}

bool ASEParser::SetBoneData(Mesh* pMesh)
{
   if (pMesh->m_IsSkinningObject == false) return false;

   VertexWeight* _nowVerW;

   for (UINT i = 0; i < pMesh->m_MeshVertices_v.size(); i++)
   {
      // �������� weight vertex
      _nowVerW = pMesh->m_WeightVertex_v[i];
      pMesh->m_MeshVertices_v[i]->m_IndicesPointer = pMesh->m_MeshVertices_v[i]->m_WeightIndex;

      if (_nowVerW->m_BoneBlendWeight_v.size() > 3)
      {
         pMesh->m_MeshVertices_v[i]->m_BW1 = _nowVerW->m_BoneBlendWeight_v[0]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW2 = _nowVerW->m_BoneBlendWeight_v[1]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW3 = _nowVerW->m_BoneBlendWeight_v[2]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[0] = (BYTE)_nowVerW->m_BoneBlendWeight_v[0]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[1] = (BYTE)_nowVerW->m_BoneBlendWeight_v[1]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[2] = (BYTE)_nowVerW->m_BoneBlendWeight_v[2]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[3] = (BYTE)_nowVerW->m_BoneBlendWeight_v[3]->m_BoneNum;
      }
      else if (_nowVerW->m_BoneBlendWeight_v.size() == 3)
      {
         pMesh->m_MeshVertices_v[i]->m_BW1 = _nowVerW->m_BoneBlendWeight_v[0]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW2 = _nowVerW->m_BoneBlendWeight_v[1]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW3 = _nowVerW->m_BoneBlendWeight_v[2]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[0] = (BYTE)_nowVerW->m_BoneBlendWeight_v[0]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[1] = (BYTE)_nowVerW->m_BoneBlendWeight_v[1]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[2] = (BYTE)_nowVerW->m_BoneBlendWeight_v[2]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[3] = 0;
      }
      else if (_nowVerW->m_BoneBlendWeight_v.size() == 2)
      {
         pMesh->m_MeshVertices_v[i]->m_BW1 = _nowVerW->m_BoneBlendWeight_v[0]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW2 = _nowVerW->m_BoneBlendWeight_v[1]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW3 = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[0] = (BYTE)_nowVerW->m_BoneBlendWeight_v[0]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[1] = (BYTE)_nowVerW->m_BoneBlendWeight_v[1]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[2] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[3] = 0;
      }
      else if (_nowVerW->m_BoneBlendWeight_v.size() == 1)
      {
         pMesh->m_MeshVertices_v[i]->m_BW1 = _nowVerW->m_BoneBlendWeight_v[0]->m_BoneWeight;
         pMesh->m_MeshVertices_v[i]->m_BW2 = 0;
         pMesh->m_MeshVertices_v[i]->m_BW3 = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[0] = (BYTE)_nowVerW->m_BoneBlendWeight_v[0]->m_BoneNum;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[1] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[2] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[3] = 0;
      }
      else if (_nowVerW->m_BoneBlendWeight_v.size() == 0)
      {
         pMesh->m_MeshVertices_v[i]->m_BW1 = 0;
         pMesh->m_MeshVertices_v[i]->m_BW2 = 0;
         pMesh->m_MeshVertices_v[i]->m_BW3 = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[0] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[1] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[2] = 0;
         pMesh->m_MeshVertices_v[i]->m_WeightIndex[3] = 0;
      }
      else
         return false;
   } // end of   for (UINT i = 0; i < _meshIter->m_MeshVertices_v.size(); i++)

   return true;
}

void ASEParser::CheckNegativeScale(Mesh* meshData)
{
   // negative scale - �븻���� ������? 3DMax���� �̷����� ������� scale�� �ݴ�Ǵ� ������ ����
   // TMRow0~2 �߿��� �ΰ��� ���� -> ������ �ϳ��� ���� -> ������ ������ negative scale ����
   Vector3 _checkNS0 = meshData->m_TMRow0;
   Vector3 _checkNS1 = meshData->m_TMRow1;
   Vector3 _checkNS2 = meshData->m_TMRow2; // row3�� translate�� �ǹ� ���ʿ�

   // row�� ����� �ǹ�, �׷��Ƿ� ���� �� ������ �ϸ� ������ ���� ������ �� �� ����(1 or -1)
   Vector3 _checkNSCross = _checkNS0.Cross(_checkNS1);
   float _checkNSDot = _checkNSCross.Dot(_checkNS2);

   if (_checkNSDot < 0)
   {
      Matrix _checkedTM(_checkNS0, _checkNS1, _checkNS2);
      _checkedTM._41 = meshData->m_TMRow3.x;
      _checkedTM._42 = meshData->m_TMRow3.y;
      _checkedTM._43 = meshData->m_TMRow3.z;

      Vector3 _s; Quaternion _r; Vector3 _t;

      _checkedTM.Decompose(_s, _r, _t);

      _s *= -1; // ������ �� ���� ������

      // �븻�鵵 ���� ������
      for (auto iter : meshData->m_MeshFaces_v)
      {
         for (UINT i = 0; i < 3; i++)
         {
            iter->m_VertexNormal[i] *= -1;
         }
      }

      // �ٽ� matrix compose
      _checkedTM =
         Matrix::CreateScale(_s) *
         Matrix::CreateFromQuaternion(_r) *
         Matrix::CreateTranslation(_t);

      meshData->m_TMRow0 = Vector3(_checkedTM._11, _checkedTM._12, _checkedTM._13);
      meshData->m_TMRow1 = Vector3(_checkedTM._21, _checkedTM._22, _checkedTM._23);
      meshData->m_TMRow2 = Vector3(_checkedTM._31, _checkedTM._32, _checkedTM._33);
      meshData->m_TMRow3 = Vector3(_checkedTM._41, _checkedTM._42, _checkedTM._43);
   }
}

Mesh* ASEParser::GetMesh(int index)
{
   return m_MeshData_v[index];
}

Animation* ASEParser::GetAnimation(string aniName)
{
   if (m_Animation_l.empty()) return nullptr;

   bool _isAnimated = false; // �̰� �� ������...? ����� �ȳ���

   // ���� �ִ��� �̸��� ���ϰ� �ִϸ��̼� ������ �����´�
   for (auto _aniIter : m_Animation_l)
   {
      if (_aniIter->m_NodeNameAni == aniName)
      {
         _isAnimated = true;
         return _aniIter;
      }
   }

   // �ִϸ��̼� ������ �������ʹ�
   if (_isAnimated == false)
   {
      return nullptr;
   }

   return nullptr;
}


// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
// ���� �� ��ū�� ���� �����Ѵ�.
// ��͸� ���� ������ '{'�� ���� ��� �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ��
// ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� '� ���'������ ����
void ASEParser::ParsingDivergeRecursiveALL(int depth)
{
   // ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
   int errorChecker = 0;
   // ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
   int listCount = 0;
   // �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
   // �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
   LONG nowToken;

   // �̰� ���ִ°���
   static int lastFaceIndex = 0;
   static int lastVerIndex = 0;

   /// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�. (, ������ : �տ����� �ϰ� �ڿ��� �̾�ض�)
   while (nowToken = m_Lexer->GetToken(m_TokenString), nowToken != TOKEND_BLOCK_END)
   {
      // �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
      // �Ѿ�� ��ū�� ���� ó�����ش�.
      static int iv = 0; // ������?

      switch (nowToken)
      {
         case TOKEND_BLOCK_START: // '{' <- �̰� ������ ��ŸƮ
         {
            ParsingDivergeRecursiveALL(depth++);
            break;
         }
         case TOKENR_HELPER_CLASS:
         {
            Parsing_String();
            break;
         }

         /// 3DSMAX_ASCIIEXPORT
         case TOKENR_3DSMAX_ASCIIEXPORT:
         {
            m_data_asciiexport = Parsing_NumberLong();
            break;
         }

         /// COMMENT
         case TOKENR_COMMENT:
         {
            Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
            //AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
            break;
         }

         /// SCENE
         case TOKENR_SCENE:
         {
            break;
         }
         case TOKENR_SCENE_FILENAME:
         {
            m_SceneData.m_FileName = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
            break;
         }
         case TOKENR_SCENE_FIRSTFRAME:
         {
            m_SceneData.m_FirstFrame = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_LASTFRAME:
         {
            m_SceneData.m_LastFrame = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_FRAMESPEED:
         {
            m_SceneData.m_FrameSpeed = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_TICKSPERFRAME:
         {
            m_SceneData.m_TicksPerFrame = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_MESHFRAMESTEP:
         {
            m_SceneData.m_MeshFrameStep = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_KEYFRAMESTEP:
         {
            m_SceneData.m_KeyFrameStep = Parsing_NumberLong();
            break;
         }
         case TOKENR_SCENE_BACKGROUND_STATIC:
         {
            m_SceneData.m_SceneBackgroundStatic = Parsing_NumberVector3();
            break;
         }
         case TOKENR_SCENE_AMBIENT_STATIC:
         {
            m_SceneData.m_SceneAmbientStatic = Parsing_NumberVector3();
            break;
         }
         case TOKENR_SCENE_ENVMAP:
         {
            // �׳� ���� ������ �о������ }�� ���ö�����
            while (nowToken = m_Lexer->GetToken(m_TokenString), nowToken != TOKEND_BLOCK_END)
            {
            }
            break;
         }

         /// MATERIAL_LIST


         /// GEOMOBJECT
         case TOKENR_GROUP:
            //	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
            break;

         case TOKENR_HELPEROBJECT:
         {
            m_ParsingMode = eHelperObject;
            Create_onemesh_to_list();
            break;
         }
         case TOKENR_SHAPEOBJECT:
         {
            m_ParsingMode = eShape;
            Create_onemesh_to_list();
            break;
         }
         case TOKENR_GEOMOBJECT:
         {
            // ���ο� �޽ð� ����ٴ� ��, ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����
            m_ParsingMode = eGeomobject;
            Create_onemesh_to_list();
            break;
         }
         case TOKENR_NODE_PARENT:
         {
            // �� ����� �θ� ����� ����.
            // �ϴ� �Է��� �ϰ�, ���߿� ��������.
            m_OneMeshData->m_IsParentExist = true;
            m_OneMeshData->m_ParentNodeName = Parsing_String();

            break;
         }
         case TOKENR_BOUNDINGBOX_MAX:
         {
            break;
         }
         case TOKENR_BOUNDINGBOX_MIN:
         {
            break;
         }
         /// Node Name�� ���� ��� �и�
         case TOKENR_NODE_NAME:
         {
            // ����ν�� ������Ʈ���� ���� �� �� �ִ� ��
            // ��忡 ���� �־�� �� ���� �ٸ���.
            if (m_ParsingMode == eAnimation)
            {
               m_Animation->m_NodeNameAni = Parsing_String();
            }
            else
            {
               m_OneMeshData->m_NodeName = Parsing_String();
            }
            break;
         }

         /// NODE_TM
         case TOKENR_NODE_TM:
         {
            // (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
            // (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

            // �Դٰ�,
            // ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
            // �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
            //if (m_onemesh->m_camera_isloadTarget) {
            //	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
            // ��ͱ����� �������̴�....

            // ������ ����� �ϳ�?
            //if (m_parsingmode == eGeomobject)

            //if (m_parsingmode == eHelperObject)

            break;
         }
         case TOKENR_INHERIT_POS: // ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
         {
            m_OneMeshData->m_InheritPos.x = Parsing_NumberFloat();
            m_OneMeshData->m_InheritPos.y = Parsing_NumberFloat();
            m_OneMeshData->m_InheritPos.z = Parsing_NumberFloat();
            break;
         }
         case TOKENR_INHERIT_ROT:
         {
            m_OneMeshData->m_InheritRot.x = Parsing_NumberFloat();
            m_OneMeshData->m_InheritRot.y = Parsing_NumberFloat();
            m_OneMeshData->m_InheritRot.z = Parsing_NumberFloat();
            break;
         }
         case TOKENR_INHERIT_SCL:
         {
            m_OneMeshData->m_InheritScl.x = Parsing_NumberFloat();
            m_OneMeshData->m_InheritScl.y = Parsing_NumberFloat();
            m_OneMeshData->m_InheritScl.z = Parsing_NumberFloat();
            break;
         }
         case TOKENR_TM_ROW0:
         {
            m_OneMeshData->m_TMRow0 = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_ROW1:
         {
            m_OneMeshData->m_TMRow2 = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_ROW2:
         {
            m_OneMeshData->m_TMRow1 = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_ROW3:
         {
            m_OneMeshData->m_TMRow3 = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_POS:
         {
            m_OneMeshData->m_TMPos = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_ROTAXIS:
         {
            m_OneMeshData->m_TMRotAxis = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_ROTANGLE:
         {
            m_OneMeshData->m_TMRotAngle = Parsing_NumberFloat();
            break;
         }
         case TOKENR_TM_SCALE:
         {
            m_OneMeshData->m_TMScale = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_SCALEAXIS:
         {
            m_OneMeshData->m_TMScaleAxis = Parsing_NumberVector3();
            break;
         }
         case TOKENR_TM_SCALEAXISANG: // ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
         {
            m_OneMeshData->m_TMScaleAxisAng = Parsing_NumberFloat();
            break;
         }

         /// Animation
         case TOKENR_TM_ANIMATION:
         {
            m_ParsingMode = eAnimation;
            m_OneMeshData->m_IsAnimated = true;
            Create_animationdata_to_list();
            break;
         }
         /// �ӽ÷� ���� �ִ� ��ū
         case TOKENR_ANIMATION_NAME:
         {
            m_Animation->m_AnimationName = Parsing_String();
            m_AnimationByName_v.push_back(m_Animation);
            break;
         }
         case TOKENR_CONTROL_POS_TRACK:
         {
            m_OneMeshData->m_IsPosAnimated = true;
            break;
         }
         case TOKENR_CONTROL_POS_SAMPLE:
         {
            AnimationPos* _nowPos = new AnimationPos();
            _nowPos->m_Frame = Parsing_NumberInt();
            _nowPos->m_Pos = Parsing_NumberVector3();
            m_Animation->m_Position_v.push_back(_nowPos);
            break;
         }
         case TOKENR_CONTROL_ROT_TRACK:
         {
            m_OneMeshData->m_IsRotAnimated = true;
            break;
         }
         case TOKENR_CONTROL_ROT_SAMPLE:
         {
            AnimationRot* _nowRot = new AnimationRot();
            _nowRot->m_Frame = Parsing_NumberInt();
            _nowRot->m_Rot = Parsing_NumberVector3();
            _nowRot->m_Angle = Parsing_NumberFloat();

            // rotQT accumulation �߰��ϱ�
            int _prevIndex = m_Animation->m_Rotation_v.size() - 1;
            _nowRot->m_AccumulatedQTRot = Quaternion::CreateFromAxisAngle(_nowRot->m_Rot, _nowRot->m_Angle);

            // ó�� ���Ŀ� QT�����ؼ� ���������
            if (_prevIndex >= 0)
            {
               // ���ʹϾ��� ��ȯ��Ģ�� �����ȵ�. ���� ���?
               _nowRot->m_AccumulatedQTRot =
                  m_Animation->m_Rotation_v[_prevIndex]->m_AccumulatedQTRot * _nowRot->m_AccumulatedQTRot;
            }

            m_Animation->m_Rotation_v.push_back(_nowRot);
            break;
         }

         /// MESH
         case TOKENR_MESH:
         {
            //m_OneMesh = m_MeshList.back();
            break;
         }
         case TOKENR_TIMEVALUE:
         {
            m_OneMeshData->m_timevalue = Parsing_NumberInt();
            break;
         }
         case TOKENR_MESH_NUMSKINWEIGHT:
            break;
         case TOKENR_MESH_NUMVERTEX:
         {
            int _count = Parsing_NumberInt();
            m_OneMeshData->m_Mesh_NumVertex = _count;

            for (int i = 0; i < _count; ++i)
            {
               m_OneMeshData->m_MeshVertices_v.push_back(new Vertex());
            }
            break;
         }
         case TOKENR_MESH_NUMFACES:
         {
            int _count = Parsing_NumberInt();
            m_OneMeshData->m_MeshNumFace = _count;

            for (int i = 0; i < _count; ++i)
            {
               m_OneMeshData->m_MeshFaces_v.push_back(new Face());
            }
            break;
         }

         /// MESH_VERTEX_LIST
         case TOKENR_MESH_VERTEX_LIST:
            // ���ؽ��� ������ ����־�� �ϴµ�
            // �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
            break;
         case TOKENR_MESH_VERTEX:
         {
            // ���ؽ� ������ �Է�
            int _index = Parsing_NumberInt();
            m_OneMeshData->m_MeshVertices_v[_index]->m_Indices = _index;
            m_OneMeshData->m_MeshVertices_v[_index]->m_Pos.x = Parsing_NumberFloat();
            m_OneMeshData->m_MeshVertices_v[_index]->m_Pos.z = Parsing_NumberFloat();
            m_OneMeshData->m_MeshVertices_v[_index]->m_Pos.y = Parsing_NumberFloat();
            break;
         }

         /// BONE
         case TOKENR_SKIN_INITTM:
            break;
         case TOKENR_MESH_NUMBONE:
         {
            Parsing_NumberInt();
            m_OneMeshData->m_IsSkinningObject = true;
            break;
         }
         case TOKENR_BONE_LIST:
         {
            m_ParsingMode = eBone_List;

            break;
         }
         case TOKENR_BONE:
         {
            // ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
            // �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
            Bone* _nowBone = new Bone();
            m_OneMeshData->m_Bone = _nowBone;
            m_OneMeshData->m_Bone_v.push_back(_nowBone);

            // Bone�� �ѹ��� �о� ����					 
            //_nowBone->m_BoneNumber = Parsing_NumberInt();
            m_OneMeshData->m_Bone->m_BoneNumber = Parsing_NumberInt();
            break;
         }
         //�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
         case TOKENR_BONE_NAME:
         {
            m_OneMeshData->m_Bone->m_BoneName = Parsing_String();
            break;
         }
         case TOKENR_BONE_PROPERTY:
            // �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
            break;
            // �������� TM_ROW0~3�� �����µ� ���� ���õ�..

         case TOKENR_MESH_WVERTEXS:
            break;

         case TOKENR_MESH_WEIGHT:
         {
            // ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
            VertexWeight* _nowVerWeight = new VertexWeight();
            m_OneMeshData->m_WeightVertex = _nowVerWeight;
            _nowVerWeight->m_NumWeightVertex = Parsing_NumberInt();

            m_OneMeshData->m_WeightVertex_v.push_back(_nowVerWeight);
            break;
         }
         case TOKENR_BONE_BLENGING_WEIGHT:
         {
            // ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
            Weight* _nowWeight = new Weight();
            m_OneMeshData->m_WeightVertex->m_TempBoneBlendWeight = _nowWeight;
            _nowWeight->m_BoneNum = Parsing_NumberInt();
            _nowWeight->m_BoneWeight = Parsing_NumberFloat();

            m_OneMeshData->m_WeightVertex->m_BoneBlendWeight_v.push_back(_nowWeight);
            break;
         }

         /// MESH_FACE_LIST
         case TOKENR_MESH_FACE_LIST:
            // 
            break;
         case TOKENR_MESH_NORMALS:
            break;
         case TOKENR_MESH_FACE:
         {
            // Face�� ��ȣ�ε�...
            int _index = Parsing_NumberInt();

            // ABC �б�
            for (int _abc = 0; _abc < 3; _abc++)
            {
               Parsing_String();
               m_OneMeshData->m_MeshFaces_v[_index]->m_VertexIndex[_abc] = Parsing_NumberInt();
            }

            // (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
            break;
         }
         case TOKENR_MESH_FACENORMAL:
         {
            int _index = Parsing_NumberInt();
            m_OneMeshData->m_MeshFaces_v[_index]->m_Normal = Parsing_NumberVector3();
            /* m_OneMesh->m_meshface[_index]->m_normal.y = Parsing_NumberFloat();
             m_OneMesh->m_meshface[_index]->m_normal.z = Parsing_NumberFloat();*/

             // �̰� ���ϴ°���
            lastFaceIndex = _index;
            lastVerIndex = 0;
            break;
         }
         case TOKENR_MESH_VERTEXNORMAL:
         {
            int _index = Parsing_NumberInt();
            //m_OneMesh->m_meshface[lastFaceIndex]->m_normalvertex[lastVerIndex].x = Parsing_NumberFloat();
            //m_OneMesh->m_meshface[lastFaceIndex]->m_normalvertex[lastVerIndex].y = Parsing_NumberFloat();
            //m_OneMesh->m_meshface[lastFaceIndex]->m_normalvertex[lastVerIndex].z = Parsing_NumberFloat();

            m_OneMeshData->m_MeshFaces_v[lastFaceIndex]->m_VertexNormal[lastVerIndex] = Parsing_NumberVector3();
            lastVerIndex++;
            break;
         }
         case TOKENR_MESH_NUMTVERTEX:
         {
            m_OneMeshData->m_Mesh_NumTVertex = Parsing_NumberInt();

            int _count = m_OneMeshData->m_Mesh_NumTVertex;
            if (_count > 0)
            {
               for (int i = 0; i < _count; i++)
               {
                  m_OneMeshData->m_MeshTVertices_v.push_back(new OneTexVertex());
               }
            }
            break;
         }
         case TOKENR_MESH_NUMCVERTEX:
         {
            break;
         }
         case TOKENR_MESH_SMOOTHING:
         {
            Parsing_NumberInt();
            break;
         }

         /// MESH_TVERTLIST
         case TOKENR_MESH_TVERTLIST:
            // �ؽ��İ� �ִ�
            m_OneMeshData->m_IsTextureSet = true;
            break;
         case TOKENR_MESH_TVERT:
         {
            // ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
            // ���ο� TVertex�� ���� ���Ϳ� �ִ´�
            int _index = Parsing_NumberInt();

            m_OneMeshData->m_MeshTVertices_v[_index]->m_u = Parsing_NumberFloat();
            m_OneMeshData->m_MeshTVertices_v[_index]->m_v = Parsing_NumberFloat();


            break;
         }
         case TOKENR_MESH_NUMTVFACES:
            break;
         case TOKENR_MESH_TFACE:
         {
            int _index = Parsing_NumberInt();
            m_OneMeshData->m_MeshFaces_v[_index]->m_TFace[0] = Parsing_NumberInt();
            m_OneMeshData->m_MeshFaces_v[_index]->m_TFace[1] = Parsing_NumberInt();
            m_OneMeshData->m_MeshFaces_v[_index]->m_TFace[2] = Parsing_NumberInt();

            break;
         }
         case TOKENR_WIREFRAME_COLOR:
         {
            m_OneMeshData->m_wireframe_color = Parsing_NumberVector3();
            break;
         }
         case TOKEND_END:
            // �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?
            //AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
            TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
            return;

            /// ���� �ƹ��͵� �ش����� ������
         default:
            // �ƹ��͵� ���� �ʴ´�.
            break;

      } // end of switch(nowtoken)

      /// ���� Ȯ�� 
      errorChecker++;

  
      if (errorChecker > 2000000)
      {
         // ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
         // ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
         TRACE("������ �鸸�� ���ҽ��ϴ�!\n");
         return;
      }

   } // end of while(nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)

   // ������� �Դٸ� while()�� ����ٴ� ��, �� ����
   // ��ȣ�� �ݾҴٴ� ���̹Ƿ� ���� (��� ��)

   return;
}


/// parsing�� ���� ������ �Լ���
// long�� �о �������ش�.
int ASEParser::Parsing_NumberLong()
{
   LONG token;
   LONG tempNumber;

   token = m_Lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
   tempNumber = strtoul(m_TokenString, NULL, 10);

   return tempNumber;
}

// float
float ASEParser::Parsing_NumberFloat()
{
   LONG token;
   float tempNumber;

   token = m_Lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
   tempNumber = (float)atof(m_TokenString);

   return tempNumber;
}

// String
LPSTR ASEParser::Parsing_String()
{
   /// m_TokenString (char[255]) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �˾Ƽ� �޾Ƶ���
   m_Lexer->GetToken(m_TokenString);

   return m_TokenString;
}

// int
int ASEParser::Parsing_NumberInt()
{
   LONG token;
   int tempNumber;

   token = m_Lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
   tempNumber = (int)atoi(m_TokenString);

   return tempNumber;
}

// 3���� Float�� ���� �ϳ���
Vector3 ASEParser::Parsing_NumberVector3()
{
   LONG token;
   Vector3 tempVector3;

   token = m_Lexer->GetToken(m_TokenString);
   tempVector3.x = (float)atof(m_TokenString);
   token = m_Lexer->GetToken(m_TokenString);
   tempVector3.z = (float)atof(m_TokenString);
   token = m_Lexer->GetToken(m_TokenString);
   tempVector3.y = (float)atof(m_TokenString);

   return tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------

// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void ASEParser::Create_onemesh_to_list()
{
   Mesh* temp = new Mesh();
   m_OneMeshData = temp;
   m_OneMeshData->m_SceneData = m_SceneData;		// Ŭ������ �� ����
   m_MeshData_v.push_back(m_OneMeshData);
}

//// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
//void ASEParser::Create_materialdata_to_list()
//{
//   ASEMaterial* temp = new ASEMaterial();
//   m_materialdata = temp;
//   m_list_materialdata.push_back(m_materialdata);
//}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void ASEParser::Create_animationdata_to_list()
{
   Animation* temp = new Animation();
   m_Animation = temp;
   m_Animation_l.push_back(m_Animation);
}

// ���� �ϳ���..
void ASEParser::Create_onevertex_to_list()
{
   Vertex* temp = new Vertex();
   m_OneMeshData->m_MeshVertices_v.push_back(temp);
}

















