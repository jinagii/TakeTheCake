#include "pch.h"

#include "IObjectBase.h"
#include "DebugCircle.h"

#include "Camera.h"
#include "DXObjectFactory.h"
#include "ObjectManager.h"
#include "Navigation.h"
#include "ClNavigation.h"
#include "Player.h"

Player::Player(IDXObjectBase* pMesh)
	: m_dTime(0.0f)
	, m_HpMax(0)
	, m_Hp(0)
	, m_IsDead(false)
	, m_bPowerOverWhelming(false)
	, m_Timer(0.0f)
	, m_BlinkTimer(0.0f)
	, m_MovePower(1.0f)
	, m_RotatePower(1.0f)
	, m_sndMove(nullptr)
	, m_sndDie(nullptr)
	, m_bVisible(true)
	, m_OffsetVec_Camera(), m_CameraZoom(1.0f), m_CameraYOffset(1.5f)
{
	m_Pos.x = 5.0f;
	m_Pos.y = 20.2f;
	m_Pos.z = -62.0f;

	Matrix matrix = Matrix::Identity;
	matrix._41 = m_Pos.x;
	matrix._42 = m_Pos.y;
	matrix._43 = m_Pos.z;
	SetTransTM(matrix);

	/// 메쉬 세팅
	SetMesh(pMesh);

	/// 충돌 타입 설정
	m_CollisionType = COLLISION_OBJECT_TYPE::Player;
	m_pMesh->GetDebugCircle()->SetDebugIndex(static_cast<int>(m_CollisionType));

	/// 자신의 콜라이더를 벡터에 추가
	AddCollider(this->GetMesh()->GetDebugCircle());

	/// 기타 초기화
	Initialize();

	// 따라다니는 카메라의 초깃값
	m_OffsetVec_Camera.x = 0;
	m_OffsetVec_Camera.y = 1.5f;
	m_OffsetVec_Camera.x = 0;

}

Player::~Player()
{
	Release();
}


void Player::Initialize()
{
	IObjectBase::Initialize();

	// Hp 최대치 설정
	m_HpMax = 5;

	// Hp 설정
	m_Hp = m_HpMax;

	// 사망여부 설정
	m_IsDead = false;

	// 무적 상태 설정
	m_bPowerOverWhelming = false;

	// 무적 타이머 설정
	m_Timer = 5.0f;

	// 무적시간일 떄 깜빡이는 정도
	m_BlinkTimer = 0.3f;

	// 이동,회전 속도 조절용 변수 초기화
	m_MovePower = 5.0f;
	m_RotatePower = 1.5f;

	// 방향 설정
	this->m_Velocity.direction = { 0.0f, 0.0f, 0.0f };

	// 각도 설정
	this->m_Angle = -1.6f;

	// 속력 설정
	this->SetMagnitude(1.0f);

	// 디버깅 circle에 인덱스 설정
	SetColliderType();

	// 사운드 연결
	m_sndMove = SoundManager::GetInstance()->FindSound(SOUND_ANT_MOVE);
	m_sndMove->soundSeek(0);


	m_sndDie = SoundManager::GetInstance()->FindSound(SOUND_ANT_DIE);
	m_sndDie->soundSeek(0);

}

void Player::Release()
{
	SAFE_DELETE(m_pMesh);
}

void Player::Update(Camera* pCamera, float dTime)
{
	// dTime 보관
	m_dTime = dTime;

	// 이동
	if (!IsDead())
	{
		Move(pCamera, dTime);
		Rotate(dTime);
	}


	// 무적 상태면
	if (m_bPowerOverWhelming)
	{
		// 타이머 작동
		m_Timer -= dTime;

		// 타이머 다 되면
		if (m_Timer <= 0.0f)
		{
			// 무적상태 해제
			m_Timer = 5.0f;
			m_bPowerOverWhelming = false;

			// 다시 보이게 설정
			m_bVisible = true;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	m_pMesh->Update(pCamera, dTime);

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_HOME))
	{
		m_CameraYOffset += 0.1f;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_END))
	{
		m_CameraYOffset -= 0.1f;
	}
}

void Player::Render()
{
	static float _timer = m_BlinkTimer;

	if (m_bPowerOverWhelming == true)
	{
		_timer -= m_dTime;

		// 시간이 되면
		if (_timer < 0.0f)
		{
			// 렌더 유무 전환
			m_bVisible = !m_bVisible;

			_timer = m_BlinkTimer;
		}
	}

	if (m_bVisible == true)
	{
		/// Render Mesh
		m_pMesh->Render();
	}
	
}

int Player::GetHp() const
{
	return m_Hp;
}

void Player::SetHp(const int hp)
{
	this->m_Hp = hp;

	if (m_Hp <= 0)
	{
		m_Hp = 0;
		m_IsDead = true;

		m_pMesh->SetStatus("dead");

		m_sndDie->soundPlay();
	}
}

bool Player::GetPowerOverWhelming() const
{
	return m_bPowerOverWhelming;
}

float Player::GetMovePower()
{
	return m_MovePower;
}

float Player::GetRotatePower()
{
	return m_RotatePower;
}

bool Player::IsDead() const
{
	return m_IsDead;
}

void Player::Move(Camera* pCamera, const float dTime)
{
	// 속도 조절
	if (KeyManager::GetInstance()->IsOnceKeyDown('H'))
	{
		m_MovePower += 0.5f;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('J'))
	{
		m_MovePower -= 0.5f;
	}

	static bool soundFlag = false;
	/// 방향키 입력 사운드 재생
	if (KeyManager::GetInstance()->InputKeyDown(VK_UP))
	{
		m_sndMove->soundSeek(0);
		m_sndMove->soundPlayLooping();
	}

	if (KeyManager::GetInstance()->InputKeyUp(VK_UP))
	{
		//soundFlag = false;
		m_sndMove->soundPause();
	}

	static Vector3 pastPos = { m_Pos.x, m_Pos.y, m_Pos.z };
	/// Navigation 업데이트
	if (m_pNavigation->m_pDXNavigation->GetIsInTriangle() == true)
	{
		pastPos = { m_TransTM._41, m_TransTM._42, m_TransTM._43 };

		/// 방향키 입력으로 움직이기
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
		{
			MoveForward(dTime, +1.0f);	// 앞으로 전진

			m_pMesh->SetStatus("walking");

		}
		else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
		{
			MoveForward(dTime, -1.0f);	// 뒤로 후진
			m_pMesh->SetStatus("walking");
		}

		else
		{
			m_pMesh->SetStatus("idle");
		}

	}

	if (m_pNavigation->m_pDXNavigation->GetIsInTriangle() == false)
	{
		Vector3 nowPos = {
			m_TransTM._41,
			m_TransTM._42,
			m_TransTM._43,
		};

		Vector3 backVec = {
			(nowPos.x - pastPos.x),
			(nowPos.y - pastPos.y),
			(nowPos.z - pastPos.z),
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


	m_pMesh->SetTransTM(m_TransTM);
}

void Player::MoveForward(const float dTime, const float dir)
{
	m_Pos.x += m_Velocity.direction.x * m_Velocity.magnitude * dTime * dir * m_MovePower;
	m_Pos.y += m_Velocity.direction.y * m_Velocity.magnitude * dTime * dir * m_MovePower;
	m_Pos.z += m_Velocity.direction.z * m_Velocity.magnitude * dTime * dir * m_MovePower;

	m_TransTM._41 = m_Pos.x;
	m_TransTM._42 = m_Pos.y;
	m_TransTM._43 = m_Pos.z;
}

void Player::Rotate(const float dTime)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
	{
		m_RotatePower += 0.5f;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('L'))
	{
		m_RotatePower -= 0.5f;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))  m_Angle -= m_RotatePower * m_Velocity.magnitude * dTime;
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT)) m_Angle += m_RotatePower * m_Velocity.magnitude * dTime;

	// 메쉬 방향 회전
	m_pMesh->SetAngle(m_Angle);

	// 오브젝트 방향 설정
	Matrix RotationTM = XMMatrixRotationY(m_Angle);
	Matrix forwardRotMat = XMMatrixRotationY(m_Angle);
	Vector3 _tempVec = { 0.0f, 0.0f, -1.0f };

	m_Velocity.direction = XMVector3TransformNormal(_tempVec, forwardRotMat);
}

void Player::AddCollider(DebugCircle* collider)
{
	m_DebugCircles.push_back(collider);
}

void Player::Hit()
{
	// 무적 상태일 때는 피격 안함.
	if (m_bPowerOverWhelming)
	{
		return;
	}

	SetHp(m_Hp - 1);

	m_bPowerOverWhelming = true;
}

void Player::OnPickCakeUp()
{
	m_CameraZoom = 1.6f;
	m_CameraYOffset = 3.6f;
}

void Player::OnStandBy()
{
	m_CameraZoom = 1.0f;
	m_CameraYOffset = 1.5f;
}

void Player::SetNavigationMesh(ClNavigation* navigation)
{
	m_pNavigation = navigation;
}


void Player::SetCameraZoom(SHORT zoomDelta)
{
	m_CameraZoom += (float)zoomDelta * 0.001f;
}

Vector3 Player::GetOffsetVec()
{
	Vector3 _forwardVec = GetMesh()->GetForwardVec();
	Vector3 _offsetVec = _forwardVec * -7.0f;
	_offsetVec.y += m_CameraYOffset;
	_offsetVec *= m_CameraZoom;

	return _offsetVec;
}

void Player::SetColliderType()
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
