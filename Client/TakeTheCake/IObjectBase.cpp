#include "pch.h"
#include "IDXObjectBase.h"
#include "IObjectBase.h"

IObjectBase::IObjectBase()
	: m_Pos(DirectX::SimpleMath::Vector3::Zero)
	, m_Velocity(DirectX::SimpleMath::Vector3::Zero, 0.0f)	// 변수 초기화 워닝 생겨서 만들어 봤는데 해결 안됌...
	, m_TransTM(Matrix::Identity)
	, m_Angle(0.0f)
	, m_CollisionType(COLLISION_OBJECT_TYPE::None)
	, m_pMesh(nullptr)
{
}

float IObjectBase::GetMagnitude() const
{
	return this->m_Velocity.magnitude;
}

void IObjectBase::SetMagnitude(const float val = 1.0f)
{
	this->m_Velocity.magnitude = val;
}

DirectX::SimpleMath::Vector3 IObjectBase::GetPos() const
{
	return this->m_Pos;
}

void IObjectBase::SetPos(const Vector3 pos = { 0.0f, 0.0f, 0.0f })
{
	this->m_Pos = pos;
	SetTransTM();
}

void IObjectBase::SetStartPos(const Vector3 pos)
{
	this->m_StartPos = pos;
	SetPos(m_StartPos);

	SetTransTM();
}

DirectX::SimpleMath::Vector3 IObjectBase::GetDirection() const
{
	return this->m_Velocity.direction;
}

void IObjectBase::SetDirection(Vector3 dir = { 0.0f, 0.0f, 0.0f })
{
	dir.Normalize();	// 정규화
	this->m_Velocity.direction = dir;
}

float IObjectBase::GetAngle() const
{
	return m_Angle;
}

Matrix IObjectBase::GetTransTM() const
{
	return m_TransTM;
}

void IObjectBase::SetTransTM()
{
	m_TransTM._41 = m_Pos.x;
	m_TransTM._42 = m_Pos.y;
	m_TransTM._43 = m_Pos.z;
}

void IObjectBase::SetTransTM(Matrix matrix)
{
	m_TransTM._41 = matrix._41;
	m_TransTM._42 = matrix._42;
	m_TransTM._43 = matrix._43;
}

IDXObjectBase* IObjectBase::GetMesh() const
{
	if (m_pMesh == nullptr)
		return nullptr;

	return m_pMesh;
}

void IObjectBase::SetMesh(IDXObjectBase* pMesh)
{
	// 이미 매쉬가 있으면
	if (m_pMesh)
	{
		// 기존 메쉬를 지우고
		SAFE_DELETE(m_pMesh);
	}

	// 새 매쉬로 변경
	m_pMesh = pMesh;
}
