#include "pch.h"
#include "IDXObjectBase.h"
#include "DebugCircle.h"
#include "Player.h"
#include "Knife.h"

Knife::Knife(IDXObjectBase* pMesh)
	: m_pTarget(nullptr)
{
	SetMesh(pMesh);

	m_CollisionType = COLLISION_OBJECT_TYPE::Knife;
	/// 자신의 콜라이더를 벡터에 추가
	AddCollider(m_pMesh->GetDebugCircle());

	// 속력 설정
	this->SetMagnitude(1.0f);

	this->Initialize();

}

Knife::~Knife()
{
	Release();
}

void Knife::Initialize()
{
	IObjectBase::Initialize();

	SetColliderType();
}

void Knife::Release()
{
	SAFE_DELETE(m_pMesh);
}

void Knife::Update(Camera* pCamera, float dTime)
{
	// 충돌 체크
	CheckCollision();

	// 회전
	Rotate(dTime);

	UpdateMesh(pCamera, dTime);
}

void Knife::Render()
{
	m_pMesh->Render();
}

void Knife::UpdateMesh(Camera* pCamera, float dTime)
{
	/////////////////////////////////////////////
	// Update Mesh
	float offsetX = m_Pos.x;
	float offsetY = m_Pos.y;
	float offsetZ = m_Pos.z;

	Matrix matrix;
	matrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);


	m_pMesh->SetTransTM(matrix);

	m_pMesh->Update(pCamera, dTime);

}

void Knife::SetTarget(IObjectBase* pTarget)
{
	// nullptr 체크
	if (!pTarget) return;

	// 타겟 설정
	this->m_pTarget = pTarget;

	Player* player = dynamic_cast<Player*>(m_pTarget);
	if (player)
	{
		AddCollider(player->GetMesh()->GetDebugCircle());
	}
}

void Knife::SetColliderType()
{
	/// 디버깅 circle에 인덱스(타입) 설정
	switch (m_CollisionType)
	{
	case COLLISION_OBJECT_TYPE::None:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::None));
		break;

	case COLLISION_OBJECT_TYPE::Player:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Player));
		break;

	case COLLISION_OBJECT_TYPE::CupCake:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::CupCake));
		break;

	case COLLISION_OBJECT_TYPE::Fire:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Fire));
		break;

	case COLLISION_OBJECT_TYPE::Knife:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Knife));
		break;

	case COLLISION_OBJECT_TYPE::Trap:
		m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(COLLISION_OBJECT_TYPE::Trap));
		break;

	default:
		break;
	}
}

void Knife::AddCollider(DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void Knife::CheckCollision()
{
	// 충돌 true / false 설정
	for (unsigned int i = 0; i < m_DebugCircles.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_DebugCircles.size(); j++)
		{
			m_DebugCircles[i]->CollisionCheck(m_DebugCircles[j]);

		}
	}

	// 무엇이랑 충돌했는지 판별하여 로직 실행
	for (size_t i = 0; i < m_DebugCircles.size(); i++)
	{
		if (m_DebugCircles[i]->GetIsCollided())
		{
			// 인덱스 확인
			int index = m_DebugCircles[i]->GetDebugIndex();

			switch (index)
			{
			case static_cast<int>(COLLISION_OBJECT_TYPE::Player):

				dynamic_cast<Player*>(m_pTarget)->Hit();

				break;

			default:
				break;
			}
		}
	}
}

void Knife::Rotate(const float dTime)
{
	m_Angle += 5.0f * m_Velocity.magnitude * dTime;

	// 메쉬 방향 회전
	m_pMesh->SetAngle(m_Angle);

	// 오브젝트 방향 설정
	Matrix RotationTM = XMMatrixRotationY(m_Angle);
	Matrix forwardRotMat = XMMatrixRotationY(m_Angle);
	Vector3 _tempVec = { 0.0f, 0.0f, -1.0f };

	m_Velocity.direction = XMVector3TransformNormal(_tempVec, forwardRotMat);
}


