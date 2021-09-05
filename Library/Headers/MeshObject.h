#pragma once

#include "DXDefine.h"
#include "d3dx11Effect.h"	
#include "StructAndConst.h"
#include "Effect.h"

#include "IDXObjectBase.h"
#include "ParsingData.h"

struct Mesh;
class Animation;
class ASEParser;
struct MeshStructure;
//class DebugCircle;


/// 일단은 ASE Parser로 불러온 단일 메쉬 클래스
/// 모델이 여러개의 메쉬로 이루어져 있을때도 고려
/// 2021.7.21 JinHak

class MeshObject : public IDXObjectBase
{
public:
	MeshObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~MeshObject();

	virtual void Initialize(ASEParser* pASEParserData);
	virtual void Update(Camera* pCamera, float dTime);
	virtual void Render();
	void BuildGeometryBuffers();
	//virtual void LoadGeometry(DXGeometryBuffer* pGB);
	//void BuildGeometryBuffers2();		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.
	
	virtual void SetDiffuseMapSRV(ID3D11ShaderResourceView* val);
	virtual void SetNormalMapSRV(ID3D11ShaderResourceView* val);

	virtual void SetStatus(string status);


	vector<MeshStructure*> GetMeshObject_v() const { return m_MeshObject_v; }

public:
	void LoadMeshDataFromParser(ASEParser* pASEParserData);

	void SetDebugCircle(float val); 

	/// Parser에 있는것보다 이 곳에 있는게 자연스럽다
	// localNode들을 재귀로 들어가서 업데이트 한다
	void UpdateNodeTM(Mesh* pMesh);
	Matrix GetParentNodeTM(Mesh* pMesh);

	/// Animation 관련	
	Matrix DecomposedLocalTM(MeshStructure* meshData, string aniName, float frame);

	/// Skinning 관련 	
	void SetBoneTM(vector<Bone*> bone_v, ASEParser* pASEParserData);
	void UpdateBoneTM(Mesh* pMesh, vector<Matrix>& finalBoneTM_v);

private:

	UINT m_VertexIdxCnt;
	Material m_Material;

	/// 빛은 외부에서 받아오도록 하자
	Vector3 m_EyePosW;

	///------------------------------------------------------------------------------
	/// 텍스쳐에서 추가된 부분
	bool m_IsSetNormalMap;
	ID3D11ShaderResourceView* m_pDiffuseMapSRV;
	ID3D11ShaderResourceView* m_pNormalMapSRV;

	Matrix m_TexTransform;

	/// 안쓰는듯
	//int m_BoxVertexOffset;
	//UINT m_BoxIndexOffset;
	//UINT m_BoxIndexCount;

	// node과제에서 추가
	//vector<Mesh*> m_MeshVec; // 아래 vec으로 수정
	vector<MeshStructure*> m_MeshObject_v; // IB, VB가 메쉬별로 따로 포함

	// animation 추가
	Animation* m_pAnimationData; // 한 모델이 여러애니를 가질수 있다 바꿔야함...

	// Skinning 추가
   vector<Bone*> m_Bone_v;

	string m_NowAnimation;
	float m_AccuFrame;


};

