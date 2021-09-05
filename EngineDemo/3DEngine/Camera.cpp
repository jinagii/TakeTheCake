#include "Camera.h"

Camera::Camera()
	: m_Position(0.0f, 0.0f, 0.0f),
	m_Right(1.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f), m_Look(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 10000.0f);
}

Camera::~Camera()
{
}

Vector3 Camera::GetPosition() const
{
	return m_Position;
}

Vector3 Camera::GetPositionXM() const
{
	return m_Position; // XMLoadFloat3(&m_Position);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position = Vector3(x, y, z);
}

void Camera::SetPosition(const Vector3& val)
{
	m_Position = val;
}

Vector3 Camera::GetRight() const
{
	return m_Right;
}

Vector3 Camera::GetRightXM() const
{
	return m_Right; // XMLoadFloat3(&m_Right);
}

Vector3 Camera::GetUp() const
{
	return m_Up;
}

Vector3 Camera::GetUpXM() const
{
	return m_Up; // XMLoadFloat3(&m_Up);
}

Vector3 Camera::GetLook() const
{
	return m_Look;
}

Vector3 Camera::GetLookXM() const
{
	return m_Look; //XMLoadFloat3(&m_Look);
}

float Camera::GetNearWndWidth() const
{
	return m_Aspect * m_NearWndHeight;
}

float Camera::GetNearWndHeight() const
{
	return m_NearWndHeight;
}

float Camera::GetFarWndWidth() const
{
	return m_Aspect * m_FarWndHeight;
}

float Camera::GetFarWndHeight() const
{
	return m_FarWndHeight;
}

float Camera::GetNearZ() const
{
	return m_NearZ;
}

float Camera::GetFarZ() const
{
	return m_FarZ;
}

float Camera::GetAspect() const
{
	return m_Aspect;
}

float Camera::GetFovY() const
{
	return m_FovY;
}

float Camera::GetFovX() const
{
	return 2.0f * atan(0.5f * GetNearWndWidth() / m_NearZ);
}

/// 인터페이스로 묶으려고 만들었다기엔 부조리한데...
//void Camera::Update(Camera* pCamera, float dTime)
//{
//}
//
//void Camera::Render()
//{
//}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	m_NearWndHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWndHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	// XMMatrixOrthographicLH for orthogonal
	/// perspective view
	Matrix _p = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	//XMStoreFloat4x4(&m_Proj, _p);

	m_Proj = _p;
}

void Camera::LookAt(Vector3 pos, Vector3 target, Vector3 worldUp)
{
	Vector3 _l = XMVector3Normalize(target - pos);
	Vector3 _r = XMVector3Normalize(XMVector3Cross(worldUp, _l));
	Vector3 _u = XMVector3Cross(_l, _r);

	m_Position = pos;
	m_Look = _l;
	m_Right = _r;
	m_Up = _u;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	Vector3 _p = pos; //XMLoadFloat3(&pos);
	Vector3 _t = target; //XMLoadFloat3(&target);
	Vector3 _u = up; //XMLoadFloat3(&up);

	LookAt(_p, _t, _u);
}

Matrix Camera::View() const
{
	return m_View; // XMLoadFloat4x4(&m_View);
}

Matrix Camera::Proj() const
{
	return m_Proj; // XMLoadFloat4x4(&m_Proj);
}

Matrix Camera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void Camera::MoveSide(float d)
{
	// m_Position += d * m_Right
	Vector3 _s = XMVectorReplicate(d);
	Vector3 _r = m_Right; 
	Vector3 _p = m_Position; 


	m_Position = XMVectorMultiplyAdd(_s, _r, _p);
}

void Camera::MoveForward(float d)
{
	// m_Position += d * m_Look
	Vector3 _s = XMVectorReplicate(d);
	Vector3 _l = m_Look; 
	Vector3 _p = m_Position;;

	m_Position = XMVectorMultiplyAdd(_s, _l, _p);
}

void Camera::MoveUpDown(float d)
{
	// m_Position += d * m_Up
	Vector3 _s = XMVectorReplicate(d);
	Vector3 _u = m_Up;
	Vector3 _p = m_Position; ;

	m_Position = XMVectorMultiplyAdd(_s, _u, _p);
}

void Camera::Pitch(float angle)
{
	Matrix _r = XMMatrixRotationAxis(m_Right, angle);

	m_Up = XMVector3TransformNormal(m_Up, _r);
	m_Look = XMVector3TransformNormal(m_Look, _r);
}

void Camera::RotateY(float angle)
{
	Matrix _r = XMMatrixRotationY(angle);

	m_Right = XMVector3TransformNormal(m_Right, _r);
	m_Up = XMVector3TransformNormal(m_Up, _r);
	m_Look = XMVector3TransformNormal(m_Look, _r);
}

void Camera::UpdateViewMatrix()
{
	Vector3 _r = m_Right; 
	Vector3 _u = m_Up; 
	Vector3 _l = m_Look; 
	Vector3 _p = m_Position; 

	// Keep camera's axes orthogonal to each other and of unit length.
	_l = XMVector3Normalize(_l);
	_u = XMVector3Normalize(XMVector3Cross(_l, _r));

	// U, L already ortho-normal, so no need to normalize cross product.
	_r = XMVector3Cross(_u, _l);

	// Fill in the view matrix entries.
	float _x = -XMVectorGetX(XMVector3Dot(_p, _r));
	float _y = -XMVectorGetX(XMVector3Dot(_p, _u));
	float _z = -XMVectorGetX(XMVector3Dot(_p, _l));

	m_Right = _r;
	m_Up = _u;
	m_Look = _l;

	m_View = Matrix(
		m_Right.x, m_Up.x, m_Look.x, 0.0f,
		m_Right.y, m_Up.y, m_Look.y, 0.0f,
		m_Right.z, m_Up.z, m_Look.z, 0.0f,
				 _x,		_y,		 _z, 1.0f);
}
