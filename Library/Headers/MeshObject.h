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


/// �ϴ��� ASE Parser�� �ҷ��� ���� �޽� Ŭ����
/// ���� �������� �޽��� �̷���� �������� ���
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
	//void BuildGeometryBuffers2();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	
	virtual void SetDiffuseMapSRV(ID3D11ShaderResourceView* val);
	virtual void SetNormalMapSRV(ID3D11ShaderResourceView* val);

	virtual void SetStatus(string status);


	vector<MeshStructure*> GetMeshObject_v() const { return m_MeshObject_v; }

public:
	void LoadMeshDataFromParser(ASEParser* pASEParserData);

	void SetDebugCircle(float val); 

	/// Parser�� �ִ°ͺ��� �� ���� �ִ°� �ڿ�������
	// localNode���� ��ͷ� ���� ������Ʈ �Ѵ�
	void UpdateNodeTM(Mesh* pMesh);
	Matrix GetParentNodeTM(Mesh* pMesh);

	/// Animation ����	
	Matrix DecomposedLocalTM(MeshStructure* meshData, string aniName, float frame);

	/// Skinning ���� 	
	void SetBoneTM(vector<Bone*> bone_v, ASEParser* pASEParserData);
	void UpdateBoneTM(Mesh* pMesh, vector<Matrix>& finalBoneTM_v);

private:

	UINT m_VertexIdxCnt;
	Material m_Material;

	/// ���� �ܺο��� �޾ƿ����� ����
	Vector3 m_EyePosW;

	///------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	bool m_IsSetNormalMap;
	ID3D11ShaderResourceView* m_pDiffuseMapSRV;
	ID3D11ShaderResourceView* m_pNormalMapSRV;

	Matrix m_TexTransform;

	/// �Ⱦ��µ�
	//int m_BoxVertexOffset;
	//UINT m_BoxIndexOffset;
	//UINT m_BoxIndexCount;

	// node�������� �߰�
	//vector<Mesh*> m_MeshVec; // �Ʒ� vec���� ����
	vector<MeshStructure*> m_MeshObject_v; // IB, VB�� �޽����� ���� ����

	// animation �߰�
	Animation* m_pAnimationData; // �� ���� �����ִϸ� ������ �ִ� �ٲ����...

	// Skinning �߰�
   vector<Bone*> m_Bone_v;

	string m_NowAnimation;
	float m_AccuFrame;


};

