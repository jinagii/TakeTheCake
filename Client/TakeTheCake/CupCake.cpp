#include "pch.h"
#include "IDXObjectBase.h"
#include "IDXObjectBase.h"
#include "DebugCircle.h"
#include "IDXObjectBase.h"
#include "Player.h"
#include "Scene03_InGame.h"
#include "CupCake.h"

CupCake::CupCake(IDXObjectBase* pMesh)
	: m_pTarget(nullptr)
	, m_IsFollowingMode(false)
{
	SetMesh(pMesh);

	m_CollisionType = COLLISION_OBJECT_TYPE::CupCake;

	/// 자신의 콜라이더를 벡터에 추가
	AddCollider(this->GetMesh()->GetDebugCircle());

	Initialize();
}

CupCake::~CupCake()
{
	Release();
}

void CupCake::Initialize()
{
	IObjectBase::Initialize();
	SetColliderType();

	m_IsFollowingMode = false;

	m_pMesh->SetScaleTM(1.0f);
}

void CupCake::Release()
{
	SAFE_DELETE(m_pMesh);
}

void CupCake::Update(Camera* pCamera, float dTime)
{
	// 충돌 체크
	CheckCollision();

	UpdateMesh(pCamera, dTime);
}

void CupCake::Render()
{
	m_pMesh->Render();
}

void CupCake::UpdateMesh(Camera* pCamera, float dTime)
{
	/////////////////////////////////////////////
	
	// Update Mesh

	float offsetX;
	float offsetY;
	float offsetZ;

	Matrix trans;	// 포지션 조정

	// 플레이어를 따라가는 모드면
	if (m_IsFollowingMode == true)
	{
		offsetX = m_pTarget->GetPos().x;
		offsetY = m_pTarget->GetPos().y + 0.5f;
		offsetZ = m_pTarget->GetPos().z;

		// 메쉬 방향 회전
		m_pMesh->SetAngle(m_pTarget->GetAngle());
		
		// 현재 스케일 조정은 안되는중... 
		/// 이제 된다. 아보카도야
		m_pMesh->SetScaleTM(0.5f);

		trans = XMMatrixTranslation(offsetX, offsetY, offsetZ);

		m_pMesh->SetTransTM(trans);

		// 메쉬 업데이트
		m_pMesh->Update(pCamera, dTime);

	}
	else
	{
		offsetX = m_Pos.x;
		offsetY = m_Pos.y;
		offsetZ = m_Pos.z;

		trans = XMMatrixTranslation(offsetX, offsetY, offsetZ);

		m_pMesh->SetTransTM(trans);

		// 메쉬 업데이트
		m_pMesh->Update(pCamera, dTime);
	}

}

void CupCake::SetTarget(IObjectBase* pTarget)
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

void CupCake::SetColliderType()
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

void CupCake::AddCollider(DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void CupCake::CheckCollision()
{
	if (m_IsFollowingMode)
	{
		// 클리어 판별
		Vector3 targetPos = m_pTarget->GetPos();
		float distance = CommonMacro::GetDistance(targetPos.x, targetPos.y, targetPos.z, 5.0f, 20.1f, -62.0f);
		if (distance < 1.0f)
		{
			Scene03_InGame::SetGameMode(GAME_MODE::Clear);
		}

		return;
	}


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

				// 스페이스바 입력 시 타겟(플레이어)을 따라가도록 함
				if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
				{
					if (Scene03_InGame::sm_AntCount >= 10)
					{
						// 개미를 10마리 이상 구했을 때에만
						m_IsFollowingMode = true;

						// 게임 모드를 케이크 집은 상태로 바꾸어준다.
						Scene03_InGame::m_GamePlayState = ePlayState::PickUpCake;

						// BGM도 바꿔준다.
						Scene03_InGame* _nowScene = dynamic_cast<Scene03_InGame*>(SceneManager::GetInstance()->GetCurrentScene());
						
						if (_nowScene != nullptr)
						{
							_nowScene->ChangeBGM(ePlayState::PickUpCake);

							_nowScene->OnPickCakeUp();

							SoundManager::GetInstance()->FindAndPlay(SOUND_CAKE_PICKUP);
						}
					}
				}

				return;;

		
			default:
				break;
			}
		}
	}
}
