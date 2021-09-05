#include "pch.h"
#include "Boid.h"


Boid::Boid()
	: m_vecFlock(nullptr)
	, m_pTarget(nullptr)
	, m_MaxSpeed(0.0f)
{
}

Boid::~Boid()
{
	Release();
}

void Boid::Initialize()
{
	/// position / velocity / acceleration 초기화
	// 시작 위치 설정
	float startPosX = CommonMacro::MakeRandomFloat(-20.0f, 20.0f);
	float startPosY = 0.0f;
	float startPosZ = CommonMacro::MakeRandomFloat(-20.0f, 20.0f);

	m_Pos = DirectX::SimpleMath::Vector3::Zero;
	m_Pos = { startPosX, startPosY, startPosZ };

	// 랜덤으로 시작 방향 x, y 의 값을 설정
	float startDirX = CommonMacro::MakeRandomFloat(-1.0f, 1.0f);
	float startDirZ = CommonMacro::MakeRandomFloat(-1.0f, 1.0f);
	//float startDirZ = sqrt(1 - (startDirX * startDirX));


	m_Velocity.direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);	
	m_Velocity.direction = DirectX::SimpleMath::Vector3(startDirX, 0.0f, startDirZ);	// y 성분은 사용하지 않는다. x, z 적당히 세팅
																			// 정규화를 하니까 수치가 몇이든 방향값만 남는다.
																			// -1 ~ -1 사이의 값으로 충분
	m_Velocity.direction.Normalize();	// 방향값 벡터 정규화

	// 힘 설정
	//this->SetMagnitude(CommonMacro::MakeRandomFloat(0.3f, 1.0f));
	this->SetMagnitude(3.0f);

	// 가속도 초기화
	m_Acceleration = DirectX::SimpleMath::Vector3::Zero;


	// 일단 지금은 의미가 없는듯..
	SetMaxSpeed(1.0f);
}

void Boid::Release()
{
	SAFE_DELETE(m_pTarget);

	// flock 무리 정보는 flock 에서 해제하자.
}

void Boid::Update(Camera* pCamera, float dTime)
{
	// Update Pos
	m_Pos += (m_Velocity.direction * m_Velocity.magnitude * dTime);

	// 가속도 초기화
	m_Acceleration *= 0.0f;

	// 테스트 시뮬레이션용 일정 범위안에 Pos 가두기
	TrappingPos();
}

void Boid::Render()
{

}

void Boid::SetFlock(std::vector<Boid*>* pFlock)
{
	this->m_vecFlock = pFlock;
}

void Boid::SetTarget(IObjectBase* pTarget)
{
	if (pTarget != nullptr)
		return;

	this->m_pTarget = pTarget;

	this->SetPos(pTarget->GetPos());
}

void Boid::Flock()
{
	/// Flock
	// 정렬 
	Vector3 alignment = Vector3::Zero;
	alignment = Align();
	//alignment.Normalize();

	// 응집
	Vector3 cohesion = Vector3::Zero;
	cohesion = Cohesion();
	//cohesion.Normalize();

	// 분리
	Vector3 separation = Vector3::Zero;
	separation = Separation();
	//separation.Normalize();

	// 가속도 계산
	m_Acceleration += alignment;
	m_Acceleration += cohesion;
	m_Acceleration += separation;
	//m_Acceleration.Normalize();			// 정규화를 하긴 해야하나?
											// 하면 각각 다해야하나?
											// 마지막에 한번 해줘야하나?
											// 상관 없나? 여러 의문이...

	// Update Velocity
	m_Velocity.direction += m_Acceleration;
	m_Velocity.direction.Normalize();	

}

DirectX::SimpleMath::Vector3 Boid::Align()
{
	/// SimpleMath 네임스페이스 사용 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////
	 
	// 로컬(지역) 검사할 범위 => 반경
	float perceptionRadius = 15.0f;

	// 반경 내에 존재하는 boid 의 수
	float total = 0.0f;

	// 조항력? Sterring
	Vector3 steering = Vector3::Zero;

	// 보이드 개체들을 순회하면서
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			 this->m_Pos.x,  this->m_Pos.y,  this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// 속도를 다 더하고
			steering += other->m_Velocity.direction;

			// 보이드 개수 증가
			total++;
		}
		
	}

	if (total > 0)
	{
		// 사이즈 만큼 나눔
		steering /= total;

		// 구한 방향으로 바로 가는 것이아니라 방향을 돌리기 위하여
		steering -= this->m_Velocity.direction;
	}
	
	return steering;
}

DirectX::SimpleMath::Vector3 Boid::Cohesion()
{
	/// SimpleMath 네임스페이스 사용 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////

	// 로컬(지역) 검사할 범위 => 반경
	float perceptionRadius = 30.0f;
	
	// 반경 내에 존재하는 boid 의 수
	float total = 0.0f;

	// 조항력? Sterring
	Vector3 steering = Vector3::Zero;


	// 보이드 개체들을 순회하면서
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			this->m_Pos.x, this->m_Pos.y, this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// 위치를 더한다
			steering += other->m_Pos;

			// 보이드 개수 증가
			total++;
		}

	}

	if (total > 0)
	{
		// 사이즈 만큼 나눔
		steering /= total;
		steering -= this->m_Pos;

		this->SetMagnitude(this->GetMaxSpeed());
		steering * -this->GetMagnitude();

		steering -= this->m_Velocity.direction;
	}
	return steering;
}

DirectX::SimpleMath::Vector3 Boid::Separation()
{
	/// SimpleMath 네임스페이스 사용 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////

	// 로컬(지역) 검사할 범위 => 반경
	float perceptionRadius = 15.0f;

	// 반경 내에 존재하는 boid 의 수
	float total = 0.0f;

	// 조항력? Sterring
	Vector3 steering = Vector3::Zero;

	// 보이드 개체들을 순회하면서
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			this->m_Pos.x, this->m_Pos.y, this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// 반대로 이동하고 싶은 벡터를 구함.
			Vector3 diff = this->m_Pos - other->m_Pos;	// other 쪽에서 this로 향하는 벡터 
			diff.Normalize();

			/// 잘 모르겠다... 분리....
			// 거리에 반비례하도록
			//diff /= (d);		// 거리가 멀수록 크기가 작아짐.
			diff = (diff * d);

			steering += diff;

			// 보이드 개수 증가
			total++;
		}

	}

	if (total > 0)
	{
		// 사이즈 만큼 나눔
		steering /= total;

		this->SetMagnitude(this->GetMaxSpeed());
		steering * -this->GetMagnitude();

		steering -= this->m_Velocity.direction;
	}

	return steering;
}

float Boid::GetMaxSpeed() const
{
	return this->m_MaxSpeed;
}

void Boid::SetMaxSpeed(const float speed)
{
	this->m_MaxSpeed = speed;
}

void Boid::TrappingPos()
{
	// 시뮬레이션 하면서 일정 범위 안에 가둬놓고 보기 위한 제어 장치

	float range = 30.0f;

	if (m_Pos.x > range)
	{
		m_Pos.x = -range;
	}

	if (m_Pos.x < -range)
	{
		m_Pos.x = range;
	}

	if (m_Pos.z > range)
	{
		m_Pos.z = -range;
	}

	if (m_Pos.z < -range)
	{
		m_Pos.z = range;
	}
}
