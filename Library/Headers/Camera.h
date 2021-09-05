#pragma once
#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"

// 카메라 클래스 DX11 종속적
class Camera : public AlignedAllocationPolicy<16>
{
public:
	Camera();
	~Camera();

	// Getter/setter
	Vector3 GetPosition() const;
	Vector3 GetPositionXM() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& val);

	Vector3 GetRight() const;
	Vector3 GetRightXM() const;
	Vector3 GetUp() const;
	Vector3 GetUpXM() const;
	Vector3 GetLook() const;
	Vector3 GetLookXM() const;

	// get near & far view 공간 좌표
	float GetNearWndWidth() const;
	float GetNearWndHeight() const;
	float GetFarWndWidth() const;
	float GetFarWndHeight() const;

	// Frustum 
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	// set frustum
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	// 카메라 시야/공간 세팅
	void LookAt(Vector3 pos, Vector3 target, Vector3 worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	Matrix View() const;
	Matrix Proj() const;
	Matrix ViewProj() const;

	void MoveSide(float d);
	void MoveForward(float d);
	void MoveUpDown(float d);

	void Pitch(float angle);
	void RotateY(float angle);

	/// 이후 뷰매트릭스를 재설정?
	void UpdateViewMatrix();


private:
	// 월드 기반 좌표 관련
	Vector3 m_Position;
	Vector3 m_Right;
	Vector3 m_Up;
	Vector3 m_Look;

	// Cache frustum properties
	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovY; // field of view
	float m_NearWndHeight;
	float m_FarWndHeight;

	// Cache View/Proj matrices.
	Matrix m_View;
	Matrix m_Proj;
};

