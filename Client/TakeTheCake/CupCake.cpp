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

	/// �ڽ��� �ݶ��̴��� ���Ϳ� �߰�
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
	// �浹 üũ
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

	Matrix trans;	// ������ ����

	// �÷��̾ ���󰡴� ����
	if (m_IsFollowingMode == true)
	{
		offsetX = m_pTarget->GetPos().x;
		offsetY = m_pTarget->GetPos().y + 0.5f;
		offsetZ = m_pTarget->GetPos().z;

		// �޽� ���� ȸ��
		m_pMesh->SetAngle(m_pTarget->GetAngle());
		
		// ���� ������ ������ �ȵǴ���... 
		/// ���� �ȴ�. �ƺ�ī����
		m_pMesh->SetScaleTM(0.5f);

		trans = XMMatrixTranslation(offsetX, offsetY, offsetZ);

		m_pMesh->SetTransTM(trans);

		// �޽� ������Ʈ
		m_pMesh->Update(pCamera, dTime);

	}
	else
	{
		offsetX = m_Pos.x;
		offsetY = m_Pos.y;
		offsetZ = m_Pos.z;

		trans = XMMatrixTranslation(offsetX, offsetY, offsetZ);

		m_pMesh->SetTransTM(trans);

		// �޽� ������Ʈ
		m_pMesh->Update(pCamera, dTime);
	}

}

void CupCake::SetTarget(IObjectBase* pTarget)
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

void CupCake::SetColliderType()
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

void CupCake::AddCollider(DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void CupCake::CheckCollision()
{
	if (m_IsFollowingMode)
	{
		// Ŭ���� �Ǻ�
		Vector3 targetPos = m_pTarget->GetPos();
		float distance = CommonMacro::GetDistance(targetPos.x, targetPos.y, targetPos.z, 5.0f, 20.1f, -62.0f);
		if (distance < 1.0f)
		{
			Scene03_InGame::SetGameMode(GAME_MODE::Clear);
		}

		return;
	}


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

				// �����̽��� �Է� �� Ÿ��(�÷��̾�)�� ���󰡵��� ��
				if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
				{
					if (Scene03_InGame::sm_AntCount >= 10)
					{
						// ���̸� 10���� �̻� ������ ������
						m_IsFollowingMode = true;

						// ���� ��带 ����ũ ���� ���·� �ٲپ��ش�.
						Scene03_InGame::m_GamePlayState = ePlayState::PickUpCake;

						// BGM�� �ٲ��ش�.
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
