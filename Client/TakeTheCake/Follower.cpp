#include "pch.h"
#include "Follower.h"
#include "Player.h"

#include "IObjectBase.h"
#include "DebugCircle.h"

#include "Camera.h"
#include "DXObjectFactory.h"
#include "ObjectManager.h"
#include "Navigation.h"
#include "ClNavigation.h"


Follower::Follower(class IDXObjectBase* pMesh)
	: m_MovePower(1.0f)
	, m_RotatePower(1.0f)
	, m_sndMove(nullptr)
	, m_sndDie(nullptr)
	, m_RandomOffset()
{
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Pos.z = 0;

	Matrix matrix = Matrix::Identity;
	matrix._41 = m_Pos.x;
	matrix._42 = m_Pos.y;
	matrix._43 = m_Pos.z;
	SetTransTM(matrix);

	/// �޽� ����
	SetMesh(pMesh);

	/// �浹 Ÿ�� ����
	m_CollisionType = COLLISION_OBJECT_TYPE::Player;
	m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(m_CollisionType));

	/// �ڽ��� �ݶ��̴��� ���Ϳ� �߰�
	AddCollider(this->GetMesh()->GetDebugCircle());

	/// ��Ÿ �ʱ�ȭ
	Initialize();

	// -1~1 �������� �� ��������
	m_RandomOffset.x = ((float)(rand() - 16384) / 16384.0f) * 1.0f;
	m_RandomOffset.y = 0;
	m_RandomOffset.z = ((float)(rand() - 16384) / 16384.0f) * 1.0f;
}

Follower::~Follower()
{
	Release();
}

void Follower::Initialize()
{
	IObjectBase::Initialize();

	m_MovePower = 5.0f;
	m_RotatePower = 1.5f;

	// ���� ����
	this->m_Velocity.direction = { 0.0f, 0.0f, 0.0f };

	// ���� ����
	this->m_Angle = -1.6f;

	// �ӷ� ����
	this->SetMagnitude(1.0f);

	// ����� circle�� �ε��� ����
	SetColliderType();

	// ���� ����
	m_sndMove = SoundManager::GetInstance()->FindSound(SOUND_ANT_MOVE);
	m_sndMove->soundSeek(0);

	m_sndDie = SoundManager::GetInstance()->FindSound(SOUND_ANT_DIE);
	m_sndDie->soundSeek(0);
}

void Follower::Release()
{
	SAFE_DELETE(m_pMesh);
}

void Follower::Update(class Camera* pCamera, float dTime)
{
	// �̵�
	Move(m_pTarget, dTime);
	Rotate(dTime);

	m_pMesh->Update(pCamera, dTime);
}

void Follower::Render()
{
	/// Render Mesh
	m_pMesh->Render();
}

void Follower::SetTarget(class Player* pPlayer)
{
	m_pTarget = pPlayer;
}

float Follower::GetMovePower()
{
	return m_MovePower;
}

float Follower::GetRotatePower()
{
	return m_RotatePower;
}

void Follower::Move(class Player* target, const float dTime)
{
	// �ӵ� ����
	if (KeyManager::GetInstance()->IsOnceKeyDown('H'))
	{
		m_MovePower += 0.5f;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('J'))
	{
		m_MovePower -= 0.5f;
	}

	/*
	static Vector3 _prevPos;

	/// Navigation ������Ʈ
	if (m_pNavigation->m_pDXNavigation->GetIsInTriangle() == true)
	{
		_prevPos = { m_TransTM._41, m_TransTM._42, m_TransTM._43 };
		
		/// �ִϸ��̼� �ٲٴ�
		//m_pMesh->SetStatus("walking");
		//m_pMesh->SetStatus("idle");
	}

	if (m_pNavigation->m_pDXNavigation->GetIsInTriangle() == false)
	{
		Vector3 nowPos = {
			m_TransTM._41,
			m_TransTM._42,
			m_TransTM._43,
		};

		Vector3 backVec = {
			(nowPos.x - _prevPos.x),
			(nowPos.y - _prevPos.y),
			(nowPos.z - _prevPos.z),
		};
		backVec.Normalize();

		SetTransTM(Matrix::CreateTranslation(
			m_TransTM._41 - backVec.x * dTime * 10.0f,
			m_TransTM._42 - backVec.y * dTime * 10.0f,
			m_TransTM._43 - backVec.z * dTime * 10.0f)
		);
	}

	m_pNavigation->m_pDXNavigation->PlayerPositionUpdate(
		{
			m_TransTM._41,
			m_TransTM._42,
			m_TransTM._43
		}
	);

	m_pNavigation->m_pDXNavigation->Update(pCamera, dTime);
	*/

	m_Angle = target->GetAngle(); //�������� �ٶ󺻴� ... ���δ�

	m_DirVec = (target->GetPos() + m_RandomOffset) - GetPos();

	if (m_DirVec.Length() < 2.0f)
	{
		m_pMesh->SetStatus("idle");

		m_DirVec.Normalize();
	}
	else
	{
		m_pMesh->SetStatus("walking");

		m_DirVec.Normalize();

		m_Pos += m_DirVec * dTime * 3.5f;
	}

	m_TransTM._41 = m_Pos.x;
	m_TransTM._42 = m_Pos.y;
	m_TransTM._43 = m_Pos.z;

	m_pMesh->SetTransTM(m_TransTM);

	m_Velocity.direction = m_DirVec;
	m_Velocity.magnitude = m_DirVec.Length();
}

void Follower::Rotate(const float dTime)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
	{
		m_RotatePower += 0.5f;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('L'))
	{
		m_RotatePower -= 0.5f;
	}

	// xz������ �� ���͸� ������ angle�� �˾Ƴ���.
	Vector3 _v1 = { m_DirVec.x, 0,m_DirVec.z };
	Vector3 _v2 = { 0, 1.0f,0 };
   Vector2 _result = XMVector2AngleBetweenVectors(_v1, m_pTarget->GetOffsetVec()); // result.x���� ����

	//m_Angle = _result.x - (MathHelper::Pi/180.0f);	// radian to degree

	//m_Angle = 90.0f;

	// �޽� ���� ȸ�� (������ �޽��� ȸ����Ŵ)
	m_pMesh->SetAngle(m_Angle);
}

void Follower::SetColliderType()
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

void Follower::AddCollider(class DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void Follower::SetNavigationMesh(class ClNavigation* navigation)
{
	m_pNavigation = navigation;
}

