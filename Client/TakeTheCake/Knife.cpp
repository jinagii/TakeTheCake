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
	/// �ڽ��� �ݶ��̴��� ���Ϳ� �߰�
	AddCollider(m_pMesh->GetDebugCircle());

	// �ӷ� ����
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
	// �浹 üũ
	CheckCollision();

	// ȸ��
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
	// nullptr üũ
	if (!pTarget) return;

	// Ÿ�� ����
	this->m_pTarget = pTarget;

	Player* player = dynamic_cast<Player*>(m_pTarget);
	if (player)
	{
		AddCollider(player->GetMesh()->GetDebugCircle());
	}
}

void Knife::SetColliderType()
{
	/// ����� circle�� �ε���(Ÿ��) ����
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
	// �浹 true / false ����
	for (unsigned int i = 0; i < m_DebugCircles.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_DebugCircles.size(); j++)
		{
			m_DebugCircles[i]->CollisionCheck(m_DebugCircles[j]);

		}
	}

	// �����̶� �浹�ߴ��� �Ǻ��Ͽ� ���� ����
	for (size_t i = 0; i < m_DebugCircles.size(); i++)
	{
		if (m_DebugCircles[i]->GetIsCollided())
		{
			// �ε��� Ȯ��
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

	// �޽� ���� ȸ��
	m_pMesh->SetAngle(m_Angle);

	// ������Ʈ ���� ����
	Matrix RotationTM = XMMatrixRotationY(m_Angle);
	Matrix forwardRotMat = XMMatrixRotationY(m_Angle);
	Vector3 _tempVec = { 0.0f, 0.0f, -1.0f };

	m_Velocity.direction = XMVector3TransformNormal(_tempVec, forwardRotMat);
}


