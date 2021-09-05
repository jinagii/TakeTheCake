#pragma once

#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"
#include "d3dx11Effect.h"

class Camera;
struct Mesh;
class Animation;
struct Bone;
struct DXGeometryBuffer;
class DebugCircle;

// ������Ʈ Ÿ�� create �Ҷ� �з� switch
enum class eDXObjectType
{
	None, ASEMesh, LineList, TriangleList, 
	Max
};

/// ���� ���� ������ �߰��߰��߰�
struct MeshStructure : public AlignedAllocationPolicy<16>
{
   Mesh* pMesh;
   UINT VertexIdxCnt;
   ID3D11Buffer* pVB;
   ID3D11Buffer* pIB;

   Animation* pAnimation;
	unordered_map<string, Animation*> Animation_um; /// �ӽ� �ִ�
   //vector<Bone*> m_Bone_v;
};

/// ��ü ������Ʈ���� �⺻ ���̽� Ŭ����
/// 2021.7.21 JinHak

class IDXObjectBase : public AlignedAllocationPolicy<16>
{
public:
	IDXObjectBase(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext); // ��ӹ��� Ŭ������ �θ�Ŭ����(this)�� ���� 
	virtual ~IDXObjectBase(); // virtual������ �ڽ�Ŭ���� �Ҹ��ڰ� ȣ���� �ȵ�!

	virtual void Initialize() {}
	virtual void Initialize(Mesh* pMesh) {}
	virtual void Update(Camera* pCamera, float dTime) {}
	virtual void Render() {}

	virtual void LoadGeometry(DXGeometryBuffer* pGB) {}

	// �ڽ� ������Ʈ�� ���� ��� or �̻��
	virtual void LoadMeshData() {}
	virtual void SetDiffuseMapSRV(ID3D11ShaderResourceView* pSRV) {}
	virtual void SetNormalMapSRV(ID3D11ShaderResourceView* pSRV) {}
	virtual void SetStatus(string status) {}
	//virtual void SetImageSRV(ID3D11ShaderResourceView* pSRV) {}

	Matrix GetWorld() { return m_World; }
	void SetWorld(Matrix val) { m_World = val; }

   Vector3 GetForwardVec() const { return m_ForwardVecOffset; }
   void SetForwardVec(Vector3 val) { m_ForwardVecOffset = val; }
	float GetAngle() const { return m_Angle; }
	void SetAngle(float val) { m_Angle = val; }
	Matrix GetScaleTM() const { return m_ScaleTM; }
	void SetScaleTM(float val) { m_ScaleTM = Matrix::CreateScale(val); }
	Matrix GetRotationTM() const { return m_RotationTM; }
	void SetRotationTM(Matrix val) { m_RotationTM = val; }
	Matrix GetTransTM() const { return m_TransTM; }
	void SetTransTM(Matrix val) { m_TransTM = val; }

	void MoveForward(float dTime, float val);
	void RotateObject(float dTime, float val);

	DebugCircle* GetDebugCircle() const { return m_pDebugCircle; }
	void SetDebugCircle(DebugCircle* val) { m_pDebugCircle = val; }



	/// �ϴ� ����Ǵ°͵鸸 protect�Ѵ�
protected: 
	// ����̽� ����
	ID3D11Device* m_pDevice;						
	ID3D11DeviceContext* m_pDeviceContext;		

	// ��� �׸����� (��, �ﰢ��)
	//ID3D11RasterizerState* m_pRenderState;

	// ���� ����
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;	

	// ��ȯ ����
	Matrix m_World;	// ���� ��ȯ ��� (����->����)
	Matrix m_View; // �þ� ��ȯ ��� (ī�޶� ��)
	Matrix m_Proj; // ���� ��ȯ ��� (����/����)

	Matrix m_ScaleTM;
	Matrix m_RotationTM;
	Matrix m_TransTM;
	float m_Angle;
	Vector3 m_ForwardVecOffset;

	// ����Ʈ ����
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* m_pTech;
	ID3DX11EffectMatrixVariable* m_pFXWorldViewProj;

	// �� ����
	DirectionalLight m_TempLight[3];
	UINT m_RenderingIndex;

	// �����
	DebugCircle* m_pDebugCircle;

};

