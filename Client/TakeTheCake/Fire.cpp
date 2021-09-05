#include "pch.h"
#include "IDXObjectBase.h"
#include "DebugCircle.h"
#include "Player.h"
#include "Fire.h"

Fire::Fire(IDXObjectBase* pMeshOff, IDXObjectBase* pMeshOn)
	: m_pTarget(nullptr)
	, m_bFireOn(false)
	, m_OnDurationTime(3.0f)
	, m_OffDurationTime(5.0f)
{
	SetMesh(pMeshOn);
	m_pFireOff = pMeshOff;

	m_CollisionType = COLLISION_OBJECT_TYPE::Fire;

	/// 자신의 콜라이더를 벡터에 추가
	AddCollider(m_pMesh->GetDebugCircle());
}

Fire::~Fire()
{
	Release();
}

void Fire::Initialize()
{	
	IObjectBase::Initialize();


	SetColliderType();
}

void Fire::Release()
{
	
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pFireOff);
	
}

void Fire::Update(Camera* pCamera, float dTime)
{
	// 타이머 감소
	m_Timer -= dTime;

	FireSwitch();
	
	CheckCollision();

	UpdateMesh(pCamera, dTime);
}

void Fire::Render()
{
	if (m_bFireOn)
	{
		m_pMesh->Render();

	}

	else
	{
		m_pFireOff->Render();
	}
}


void Fire::UpdateMesh(Camera* pCamera, float dTime)
{
	/////////////////////////////////////////////
	// Update Mesh
	float offsetX = m_Pos.x;
	float offsetY = m_Pos.y;
	float offsetZ = m_Pos.z;

	Matrix matrix;
	matrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);

	if (m_bFireOn)
	{
		m_pMesh->SetTransTM(matrix);

		m_pMesh->Update(pCamera, dTime);
	}

	else
	{
		m_pFireOff->SetTransTM(matrix);
		m_pFireOff->Update(pCamera, dTime);


		// 어쩔 수 없다... 꺼져있을 때 FireOn 매쉬의 위치를 잠시 저 멀리 보내자
		Matrix transTM;
		transTM = XMMatrixTranslation(offsetX + 100, offsetY, offsetZ);
		m_pMesh->SetTransTM(transTM);
		m_pMesh->Update(pCamera, dTime);
	}
	
}


void Fire::SetTarget(IObjectBase* pTarget)
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

void Fire::SetColliderType()
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

void Fire::AddCollider(DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void Fire::CheckCollision()
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



void Fire::FireSwitch()
{
	// 타이머가 다 됐으면
	if (m_Timer <= 0.0f)
	{
		// 모드 변경(스위치)
		m_bFireOn = !m_bFireOn;

		// 타이머 재설정
		(m_bFireOn == true) ? m_Timer = m_OnDurationTime : 
							  m_Timer = m_OffDurationTime;
	}
}
