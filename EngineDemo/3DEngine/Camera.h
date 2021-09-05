#pragma once
#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"

// ī�޶� Ŭ���� DX11 ������
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

	// get near & far view ���� ��ǥ
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
	// ī�޶� �þ�/���� ����
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

	/// ���� ���Ʈ������ �缳��?
	void UpdateViewMatrix();


private:
	// ���� ��� ��ǥ ����
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

