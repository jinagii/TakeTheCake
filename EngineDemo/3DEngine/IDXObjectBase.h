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

// 오브젝트 타입 create 할때 분류 switch
enum class eDXObjectType
{
	None, ASEMesh, LineList, TriangleList, 
	Max
};

/// 모델의 구성 변수들 추가추가추가
struct MeshStructure : public AlignedAllocationPolicy<16>
{
   Mesh* pMesh;
   UINT VertexIdxCnt;
   ID3D11Buffer* pVB;
   ID3D11Buffer* pIB;

   Animation* pAnimation;
	unordered_map<string, Animation*> Animation_um; /// 임시 애니
   //vector<Bone*> m_Bone_v;
};

/// 전체 오브젝트들의 기본 베이스 클래스
/// 2021.7.21 JinHak

class IDXObjectBase : public AlignedAllocationPolicy<16>
{
public:
	IDXObjectBase(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext); // 상속받은 클래스의 부모클래스(this)가 생성 
	virtual ~IDXObjectBase(); // virtual없으면 자식클래스 소멸자가 호출이 안됨!

	virtual void Initialize() {}
	virtual void Initialize(Mesh* pMesh) {}
	virtual void Update(Camera* pCamera, float dTime) {}
	virtual void Render() {}

	virtual void LoadGeometry(DXGeometryBuffer* pGB) {}

	// 자식 오브젝트에 따라 사용 or 미사용
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



	/// 일단 공통되는것들만 protect한다
protected: 
	// 디바이스 관련
	ID3D11Device* m_pDevice;						
	ID3D11DeviceContext* m_pDeviceContext;		

	// 어떻게 그릴건지 (선, 삼각형)
	//ID3D11RasterizerState* m_pRenderState;

	// 정점 관련
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;	

	// 변환 관련
	Matrix m_World;	// 월드 변환 행렬 (로컬->월드)
	Matrix m_View; // 시야 변환 행렬 (카메라 뷰)
	Matrix m_Proj; // 투영 변환 행렬 (원근/직교)

	Matrix m_ScaleTM;
	Matrix m_RotationTM;
	Matrix m_TransTM;
	float m_Angle;
	Vector3 m_ForwardVecOffset;

	// 이펙트 관련
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* m_pTech;
	ID3DX11EffectMatrixVariable* m_pFXWorldViewProj;

	// 빛 관련
	DirectionalLight m_TempLight[3];
	UINT m_RenderingIndex;

	// 디버그
	DebugCircle* m_pDebugCircle;

};

