#include "Camera.h"
#include "ASEParser.h"
#include "DXRenderState.h"
#include "Animation.h"
#include "StructAndConst.h"
#include "DebugCircle.h"

#include "MeshObject.h"

#include "DDSTextureLoader.h"


MeshObject::MeshObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
   : IDXObjectBase(pDevice, pDeviceContext)
   , m_VertexIdxCnt(0)
   , m_IsSetNormalMap(false)
   , m_pDiffuseMapSRV(0)
   , m_pNormalMapSRV()
   , m_EyePosW(0.0f, 0.0f, 0.0f)
   , m_pAnimationData()
   , m_NowAnimation("idle")
   , m_AccuFrame(0.0f)

{
   m_TexTransform = XMMatrixIdentity();
}

MeshObject::~MeshObject()
{
   ReleaseCOM(m_pDiffuseMapSRV);
   ReleaseCOM(m_pNormalMapSRV);


}

void MeshObject::Initialize(ASEParser* pASEParserData)
{
   // 표면 머티리얼 설정
   m_Material.Ambient = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
   m_Material.Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
   m_Material.Specular = Vector4(0.8f, 0.8f, 0.8f, 16.0f);

  
   // MeshObject니까 init할 때 meshdata 등을 세팅해주자
   LoadMeshDataFromParser(pASEParserData);
}

void MeshObject::Update(Camera* pCamera, float dTime) // 요 부분에서 결국 카메라가 최종적으로 쓰이니깐
{
   // NodeTM 업데이트 - 애니메이션

   
   for (MeshStructure* _meshIter : m_MeshObject_v)
   {
      if (_meshIter->pAnimation == nullptr)
      {
         break;
      }

      Matrix _aniMatrix = DecomposedLocalTM(_meshIter, m_NowAnimation, dTime);          

      _meshIter->pMesh->m_LocalTM = _aniMatrix;

      UpdateNodeTM(_meshIter->pMesh);
   }

   m_View = pCamera->View();
   m_Proj = pCamera->Proj();
   m_EyePosW = pCamera->GetPosition(); // 카메라 위치

   // Switch the number of lights based on key presses.
   if (GetAsyncKeyState('1') & 0x8000)
   {
      m_RenderingIndex = 1;
   }
   if (GetAsyncKeyState('2') & 0x8000)
   {
      m_RenderingIndex = 2;
   }
   if (GetAsyncKeyState('3') & 0x8000)
   {
      m_RenderingIndex = 3;
   }


   // 임시로 y축으로만 회전하게 업데이트
   m_RotationTM = XMMatrixRotationY(m_Angle);
   Matrix _forwardRotMat = XMMatrixRotationY(m_Angle);
   Vector3 _tempVec = { 0.0f, 0.0f, -1.0f };
   m_ForwardVecOffset = XMVector3TransformNormal(_tempVec, _forwardRotMat);

   // 월드행렬을 업데이트함
   m_World = m_ScaleTM * m_RotationTM * m_TransTM;

   // Debug
   m_pDebugCircle->Update(pCamera, m_Angle, { m_TransTM._41,m_TransTM._42,m_TransTM._43 });
}

void MeshObject::Render()
{
   for (UINT i = 0; i < m_MeshObject_v.size(); i++)
   {
      // 입력 배치 객체 셋팅
      m_pDeviceContext->IASetInputLayout(InputLayout::PosNorTex);
      m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      // 렌더 스테이트
      m_pDeviceContext->RSSetState(DXRenderState::g_SolidFrame);

      // 버텍스버퍼와 인덱스버퍼 셋팅
      UINT _stride = sizeof(PosNorTex);
      UINT _offset = 0;

      m_pDeviceContext->IASetVertexBuffers(0, 1, &m_MeshObject_v[i]->pVB, &_stride, &_offset);
      m_pDeviceContext->IASetIndexBuffer(m_MeshObject_v[i]->pIB, DXGI_FORMAT_R32_UINT, 0);

      /// WVP TM등을 셋팅      
      Matrix _view = m_View;
      Matrix _proj = m_Proj;
      Matrix _world = m_World;
      /// Optimized된 mesh들의 NodeTM들을 world에 곱해서 렌더
      _world = m_MeshObject_v[i]->pMesh->m_NodeTM * _world;
      Matrix _worldViewProj = _world * _view * _proj;

      // 월드의 역행렬
      Matrix _worldInvTranspose = MathHelper::InverseTranspose(_world);

      //StaticEffect::BasicFX->SetDirLights(m_TempLight);
      StaticEffect::BasicFX->SetEyePosW(m_EyePosW);

      // Figure out which technique to use.
      switch (m_RenderingIndex)
      {
         case 1:
            m_pTech = StaticEffect::BasicFX->Light0TexTech; // 텍스쳐
            break;
         //case 2:
         //   m_pTech = StaticEffect::BasicFX->Light1Tech; // 메쉬만
         //   break;
      }

      // 테크닉은...
      D3DX11_TECHNIQUE_DESC _techDesc;
      m_pTech->GetDesc(&_techDesc);
      for (UINT j = 0; j < _techDesc.Passes; ++j)
      {
         StaticEffect::BasicFX->SetWorld(_world);
         StaticEffect::BasicFX->SetWorldInvTranspose(_worldInvTranspose);
         StaticEffect::BasicFX->SetWorldViewProj(_worldViewProj);
         StaticEffect::BasicFX->SetTexTransform(m_TexTransform);
         StaticEffect::BasicFX->SetMaterial(m_Material);
         StaticEffect::BasicFX->SetDiffuseMap(m_pDiffuseMapSRV);

         //if (_finalBoneTM_v.size() > 0)
         //{
         //   StaticEffect::SkinnedMeshFX->SetBoneTransforms(&(_finalBoneTM_v[0]), _finalBoneTM_v.size());
         //}

         m_pTech->GetPassByIndex(j)->Apply(0, m_pDeviceContext);
         m_pDeviceContext->DrawIndexed(m_MeshObject_v[i]->VertexIdxCnt, 0, 0);
      }
   }

   m_pDebugCircle->Render();
}

void MeshObject::BuildGeometryBuffers()
{
   for (UINT j = 0; j < m_MeshObject_v.size(); j++)
   {
      UINT _vCount = 0;
      UINT _iCount = 0;

      if (m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v.size() <= 0)
      {
         continue;
      }

      _vCount = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v.size();
      vector<PosNorTex> _vertices(_vCount);

      for (UINT i = 0; i < _vCount; i++)
      {
         _vertices[i].Pos.x = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Pos.x;
         _vertices[i].Pos.y = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Pos.y;
         _vertices[i].Pos.z = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Pos.z;

         _vertices[i].Normal.x = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Normal.x;
         _vertices[i].Normal.y = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Normal.y;
         _vertices[i].Normal.z = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_Normal.z;

         _vertices[i].Tex.x =        m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_U;
         _vertices[i].Tex.y = 1.0f - m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_V; // T값이 뒤집힙

         // 스키닝용 weight
         /*_vertices[i].Weights.x = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_BW1;
         _vertices[i].Weights.y = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_BW2;
         _vertices[i].Weights.z = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_BW3;

         _vertices[i].BoneIndices[0] = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_WeightIndex[0];
         _vertices[i].BoneIndices[1] = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_WeightIndex[1];
         _vertices[i].BoneIndices[2] = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_WeightIndex[2];
         _vertices[i].BoneIndices[3] = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[i]->m_WeightIndex[3];*/
      }

      _iCount = m_MeshObject_v[j]->pMesh->m_MeshNumFace;

      m_MeshObject_v[j]->VertexIdxCnt = 3 * _iCount; // triangle 
      vector<UINT> _indices(m_MeshObject_v[j]->VertexIdxCnt);

      for (UINT i = 0; i < _iCount; ++i)
      {
         _indices[i * 3 + 0] = m_MeshObject_v[j]->pMesh->m_OptimizedIndices[i].Index[0];
         _indices[i * 3 + 1] = m_MeshObject_v[j]->pMesh->m_OptimizedIndices[i].Index[2];
         _indices[i * 3 + 2] = m_MeshObject_v[j]->pMesh->m_OptimizedIndices[i].Index[1];
      }

      /*
      /// Tanget for NormalMap 안해도 노말맵이 된다? dds에 정보가있나? 왜 되지?
      for (UINT i = 0; i < m_MeshObject_v[j]->pMesh->m_MeshFaces_v.size(); i++)
      {
         // 메쉬를 이루는 버텍스 인덱스를 가져온다
         UINT _verIdx0 = m_MeshObject_v[j]->pMesh->m_MeshFaces_v[i]->m_VertexIndex[0];
         UINT _verIdx1 = m_MeshObject_v[j]->pMesh->m_MeshFaces_v[i]->m_VertexIndex[1];
         UINT _verIdx2 = m_MeshObject_v[j]->pMesh->m_MeshFaces_v[i]->m_VertexIndex[2];

         // 위의 인덱스의 포지션 값을 가져온다
         Vector3 _verPos0 = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx0]->m_Pos;
         Vector3 _verPos1 = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx1]->m_Pos;
         Vector3 _verPos2 = m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx2]->m_Pos;

         // 포지션 값을 빼서 삼각형 면의 벡터를 만듬
         Vector3 _vec1 = _verPos1 - _verPos0;
         Vector3 _vec2 = _verPos2 - _verPos0;

         Vector2 _t0 = Vector2(
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx0]->m_U,
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx0]->m_V);
         Vector2 _t1 = Vector2(
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx1]->m_U,
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx1]->m_V);
         Vector2 _t2 = Vector2(
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx2]->m_U,
            m_MeshObject_v[j]->pMesh->m_OptimizedVertices_v[_verIdx2]->m_V);

         // u, v 벡터 만듬
         float _x1 = _t1.x - _t0.x; // u
         float _x2 = _t2.x - _t0.x;
         float _y1 = _t1.y - _t0.y; // v
         float _y2 = _t2.y - _t0.y;

         // 역함수를 구해서 TBN을 구한다. 정확히는 T만 있으면 됨
         float _det = 1.0f / (_x1 * _y2 - _x2 * _y1);
         Vector3 _tan = (_vec1 * _y2 - _vec2 * _y1) * _det;

         Vector3 _normalizedTan;
         _tan.Normalize(_normalizedTan);

         _vertices[_verIdx0].TangentU += _tan;
         _vertices[_verIdx1].TangentU += _tan;
         _vertices[_verIdx2].TangentU += _tan;
      }
      */

      D3D11_BUFFER_DESC _vBD;
      _vBD.Usage = D3D11_USAGE_IMMUTABLE;
      _vBD.ByteWidth = sizeof(PosNorTex) * _vCount;
      _vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      _vBD.CPUAccessFlags = 0;
      _vBD.MiscFlags = 0;
      D3D11_SUBRESOURCE_DATA _vInitData;
      _vInitData.pSysMem = &_vertices[0];
      HR(m_pDevice->CreateBuffer(&_vBD, &_vInitData, &m_MeshObject_v[j]->pVB));

      // Pack the indices of all the meshes into one index buffer.
      D3D11_BUFFER_DESC _iBD;
      _iBD.Usage = D3D11_USAGE_IMMUTABLE;
      _iBD.ByteWidth = sizeof(UINT) * m_MeshObject_v[j]->VertexIdxCnt;
      _iBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
      _iBD.CPUAccessFlags = 0;
      _iBD.MiscFlags = 0;
      D3D11_SUBRESOURCE_DATA _iInitData;
      _iInitData.pSysMem = &_indices[0];
      HR(m_pDevice->CreateBuffer(&_iBD, &_iInitData, &m_MeshObject_v[j]->pIB));
   }
}

void MeshObject::SetDiffuseMapSRV(ID3D11ShaderResourceView* val)
{
   m_pDiffuseMapSRV = val;
}

void MeshObject::SetNormalMapSRV(ID3D11ShaderResourceView* val)
{
   m_pNormalMapSRV = val;
}

void MeshObject::SetStatus(string status)
{
   m_NowAnimation = status;
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void MeshObject::LoadMeshDataFromParser(ASEParser* pASEParserData)
{
   //for (UINT i = 0; i < pASEParserData->m_MeshData_v.size(); i++)
   for (Mesh* _meshDataIter : pASEParserData->m_MeshData_v)
   {
      MeshStructure* _newMeshStruct = new MeshStructure();

      _newMeshStruct->pMesh = _meshDataIter;

      // Animation도 세팅해준다
      if (_meshDataIter->m_IsAnimated == true)
      {
         for (Animation* _aniIter : pASEParserData->m_Animation_l)
         {
            if (_aniIter->m_NodeNameAni == _meshDataIter->m_NodeName)
            {
               _newMeshStruct->pAnimation = _aniIter;
               _newMeshStruct->Animation_um.insert({ _newMeshStruct->pAnimation->m_AnimationName, _newMeshStruct->pAnimation });
            }
         }
      }

      // BoneData도 넣어준다
      //if (_meshDataIter->m_IsSkinningObject == true)
      //{
      //   //_newMeshStruct->pMesh->m_IsBone = true;
      //   //_newMeshStruct->m_Bone_v = _meshDataIter->m_Bone_v;
      //   //_newMeshStruct->pMesh->m_Bone_v = _meshDataIter->m_Bone_v;
      //   SetBoneTM(_newMeshStruct->pMesh->m_Bone_v, pASEParserData);
      //}

      m_MeshObject_v.push_back(_newMeshStruct);
   }
}

void MeshObject::SetDebugCircle(float val)
{  
   m_pDebugCircle = new DebugCircle(m_pDevice, m_pDeviceContext, m_World, val);
   //m_pDebugCircle->Initialize();
   m_pDebugCircle->BuildGeometryBuffer();
}

void MeshObject::UpdateNodeTM(Mesh* pMesh)
{
   if (pMesh->m_IsParentExist == true && pMesh->m_pParentMesh != nullptr)
   {
      pMesh->m_NodeTM = pMesh->m_LocalTM * GetParentNodeTM(pMesh->m_pParentMesh);
   }
   else
   {
      pMesh->m_NodeTM = pMesh->m_LocalTM;
   }
}

Matrix MeshObject::GetParentNodeTM(Mesh* pMesh)
{
   if (pMesh->m_IsParentExist == true && pMesh->m_pParentMesh != nullptr)
   {
      return pMesh->m_LocalTM * GetParentNodeTM(pMesh->m_pParentMesh);
   }
   else
   {
      return pMesh->m_LocalTM;
   }
}

Matrix MeshObject::DecomposedLocalTM(MeshStructure* meshData, string aniName, float frame)
{
   // 보간용 index
   static UINT _currRotIdx = 0;
   static UINT _nextRotIdx = 0;
   static UINT _currPosIdx = 0;
   static UINT _nextPosIdx = 0;

   // 프레임 조절

   // 현재 애니메이션 세팅


   //if (_accumFrame > meshData->pMesh->m_SceneData.m_LastFrame * meshData->pMesh->m_SceneData.m_TicksPerFrame)
   // 이래선 안되는데... 시급하니 넘나 하드코딩...
   float _frameIdx = 0.0f;

   if (aniName == "idle")
   {
      _frameIdx = 9600.0f;
   }
   if (aniName == "walking")
   {
      _frameIdx = 2560.0f; // 2560
   }
   if (aniName == "interaction")
   {
      _frameIdx = 3840.0f; // 3840
   }
   if (aniName == "dead")
   {
      _frameIdx = 20000.0f; // 3840
   }
   //else
   //{
   //   _frameIdx = meshData->pMesh->m_SceneData.m_LastFrame * meshData->pMesh->m_SceneData.m_TicksPerFrame;
   //}
   
   m_AccuFrame += frame * 3000.0f;
   if(m_AccuFrame > _frameIdx)
   {
      m_AccuFrame = 0.0f;
      _currRotIdx = 0;
      _nextRotIdx = 0;
      _currPosIdx = 0;
      _nextPosIdx = 0;
   }

   // Decompose용 
   Vector3 _decomScl; // 기본 local값으로 되나?
   Quaternion _decomRot;
   Vector3 _decomTrans;
   // decompose로 T R S 나눠서 나중에 합산해서 다시 복원해야한다
   //_decompLocalTM.Decompose(_decomScl, _decomRot, _decomTrans);
   meshData->pMesh->m_LocalTM.Decompose(_decomScl, _decomRot, _decomTrans);

   // Decompose 후 Compose하기위한 matrices
   Matrix _sclMat, _rotMat, _traMat;
   // 결과 decomposed TM
   Matrix _decompLocalTM;

   // 애니메이션 없으면 그냥 기본 localTM 쓴다
   if (meshData->pAnimation == nullptr)
   {
      return meshData->pMesh->m_LocalTM;
   }

   if (aniName != "")
   {
      meshData->pAnimation = meshData->Animation_um.at(aniName);
   }


   // position 
   if (meshData->pAnimation->m_Position_v.size() != 0)
   {
      for (UINT j = 0; j < meshData->pAnimation->m_Position_v.size(); j++)
      {
         if (j == meshData->pAnimation->m_Position_v.size() - 1)
         {
            _currPosIdx = meshData->pAnimation->m_Position_v.size() - 1;
            _nextPosIdx = 0;
            break;
         }
         else if (meshData->pAnimation->m_Position_v[j]->m_Frame <= m_AccuFrame &&
            meshData->pAnimation->m_Position_v[j + 1]->m_Frame >= m_AccuFrame)
         {
            _currPosIdx = j;
            _nextPosIdx = j + 1;
            break;
         }
      }
   }

   // position lerp
   if (_currPosIdx < meshData->pAnimation->m_Position_v.size())
   {
      float _gapFrame =
         meshData->pAnimation->m_Position_v[_nextPosIdx]->m_Frame -
         meshData->pAnimation->m_Position_v[_currPosIdx]->m_Frame;
      float _nowFrame =
         m_AccuFrame - meshData->pAnimation->m_Position_v[_currPosIdx]->m_Frame;

      _decomTrans = Vector3::Lerp(
         meshData->pAnimation->m_Position_v[_currPosIdx]->m_Pos,
         meshData->pAnimation->m_Position_v[_nextPosIdx]->m_Pos,
         _nowFrame / _gapFrame);
   }

   // Rotation
   // Mesh의 frame값을 비교해서 시간에 맞는 TM을 만들어준다
   if (meshData->pAnimation->m_Rotation_v.size() != 0)
   {
      for (UINT i = 0; i < meshData->pAnimation->m_Rotation_v.size(); i++)
      {
         // frame값이 같으면 accuROT 쓴다
         /// 마지막 인덱스 예외처리 깔끔하게 하는방법이 뭘까
         if (i == meshData->pAnimation->m_Rotation_v.size() - 1)
         {
            _currRotIdx = meshData->pAnimation->m_Rotation_v.size() - 1;
            _nextRotIdx = 0;
            break;
         }
         else if (meshData->pAnimation->m_Rotation_v[i]->m_Frame <= m_AccuFrame &&
            meshData->pAnimation->m_Rotation_v[i + 1]->m_Frame >= m_AccuFrame)
         {
            _currRotIdx = i;
            _nextRotIdx = i + 1;
            break;
         }
      }
   }

   // Rotation lerp 
   if (_currRotIdx < meshData->pAnimation->m_Rotation_v.size())
   {
      float _gapFrame =
         meshData->pAnimation->m_Rotation_v[_nextRotIdx]->m_Frame -
         meshData->pAnimation->m_Rotation_v[_currRotIdx]->m_Frame;
      float _nowFrame =
         m_AccuFrame - meshData->pAnimation->m_Rotation_v[_currRotIdx]->m_Frame;

      _decomRot = XMQuaternionSlerp(
         meshData->pAnimation->m_Rotation_v[_currRotIdx]->m_AccumulatedQTRot,
         meshData->pAnimation->m_Rotation_v[_nextRotIdx]->m_AccumulatedQTRot,
         _nowFrame / _gapFrame);
   }

   _sclMat = Matrix::CreateScale(_decomScl);
   _rotMat = Matrix::CreateFromQuaternion(_decomRot);
   _traMat = Matrix::CreateTranslation(_decomTrans);

   //Trace((TCHAR*)L"aniName: %s, accuFrame: %f, frameIdx: %f", aniName, _accumFrame, _frameIdx);

   //_result = _traMat * _rotMat * _sclMat; // TRS? -> OpenGL
   _decompLocalTM = _sclMat * _rotMat * _traMat; // SRT? -> DirectX

   return _decompLocalTM;
}

void MeshObject::SetBoneTM(vector<Bone*> bone_v, ASEParser* pASEParserData)
{
   // bone을 돌면서 이름이 같은 bone을 찾기
   for (Bone* _boneIter : bone_v)
   {
      // parserData를 돌면서 찾는다
      for (Mesh* _dataIter : pASEParserData->m_MeshData_v)
      {
         if (_boneIter->m_BoneName == _dataIter->m_NodeName)
         {
            _dataIter->m_IsBone = true;

            _boneIter->m_boneTM_NodeTM = &_dataIter->m_LocalTM;
            _boneIter->m_boneTM_WorldTM = &_dataIter->m_NodeTM;
         }
      }
   }
}

void MeshObject::UpdateBoneTM(Mesh* pMesh, vector<Matrix>& finalBoneTM_v)
{
   /// 스키닝의 TM 구하기

   Matrix _identityTM = XMMatrixIdentity();

   Matrix _boneOffsetTM;
   Matrix _boneWorldTM;
   Matrix _boneNodeTM;

   Matrix _skinWorldTM;
   Matrix _skinWorldTMInverse;
   Matrix _boneOffsetTMInverse;

   Matrix _finalBoneTM;


   Bone* _tempBone;

   //	각 Bone의 Matrix를 구한다.
   //	스키닝의 TM 구하기
   for (Bone* _boneIter : pMesh->m_Bone_v)
   {
      _tempBone = _boneIter;

      // TM을 이미 구해 놓은 상태이다 (정확히는 구해져 있다 - 포인터를 가지고 있으므로)
      _boneWorldTM = *_tempBone->m_boneTM_WorldTM;
      //_boneWorldTM	= _identityTM;

      _boneNodeTM = *_tempBone->m_boneTM_NodeTM;
      //_boneNodeTM		= _identityTM;

      _skinWorldTM = XMLoadFloat4x4(&m_World);
      //_skinWorldTM	= _identityTM;

      _skinWorldTMInverse = XMMatrixInverse(nullptr, _skinWorldTM);

      _boneOffsetTM = _boneNodeTM * _skinWorldTMInverse;
      _boneOffsetTMInverse = XMMatrixInverse(nullptr, _boneOffsetTM);

      _finalBoneTM = _boneOffsetTMInverse * _boneWorldTM;

      finalBoneTM_v.push_back(_finalBoneTM);
   }

}
