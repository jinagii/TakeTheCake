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
	/// position / velocity / acceleration �ʱ�ȭ
	// ���� ��ġ ����
	float startPosX = CommonMacro::MakeRandomFloat(-20.0f, 20.0f);
	float startPosY = 0.0f;
	float startPosZ = CommonMacro::MakeRandomFloat(-20.0f, 20.0f);

	m_Pos = DirectX::SimpleMath::Vector3::Zero;
	m_Pos = { startPosX, startPosY, startPosZ };

	// �������� ���� ���� x, y �� ���� ����
	float startDirX = CommonMacro::MakeRandomFloat(-1.0f, 1.0f);
	float startDirZ = CommonMacro::MakeRandomFloat(-1.0f, 1.0f);
	//float startDirZ = sqrt(1 - (startDirX * startDirX));


	m_Velocity.direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);	
	m_Velocity.direction = DirectX::SimpleMath::Vector3(startDirX, 0.0f, startDirZ);	// y ������ ������� �ʴ´�. x, z ������ ����
																			// ����ȭ�� �ϴϱ� ��ġ�� ���̵� ���Ⱚ�� ���´�.
																			// -1 ~ -1 ������ ������ ���
	m_Velocity.direction.Normalize();	// ���Ⱚ ���� ����ȭ

	// �� ����
	//this->SetMagnitude(CommonMacro::MakeRandomFloat(0.3f, 1.0f));
	this->SetMagnitude(3.0f);

	// ���ӵ� �ʱ�ȭ
	m_Acceleration = DirectX::SimpleMath::Vector3::Zero;


	// �ϴ� ������ �ǹ̰� ���µ�..
	SetMaxSpeed(1.0f);
}

void Boid::Release()
{
	SAFE_DELETE(m_pTarget);

	// flock ���� ������ flock ���� ��������.
}

void Boid::Update(Camera* pCamera, float dTime)
{
	// Update Pos
	m_Pos += (m_Velocity.direction * m_Velocity.magnitude * dTime);

	// ���ӵ� �ʱ�ȭ
	m_Acceleration *= 0.0f;

	// �׽�Ʈ �ùķ��̼ǿ� ���� �����ȿ� Pos ���α�
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
	// ���� 
	Vector3 alignment = Vector3::Zero;
	alignment = Align();
	//alignment.Normalize();

	// ����
	Vector3 cohesion = Vector3::Zero;
	cohesion = Cohesion();
	//cohesion.Normalize();

	// �и�
	Vector3 separation = Vector3::Zero;
	separation = Separation();
	//separation.Normalize();

	// ���ӵ� ���
	m_Acceleration += alignment;
	m_Acceleration += cohesion;
	m_Acceleration += separation;
	//m_Acceleration.Normalize();			// ����ȭ�� �ϱ� �ؾ��ϳ�?
											// �ϸ� ���� ���ؾ��ϳ�?
											// �������� �ѹ� ������ϳ�?
											// ��� ����? ���� �ǹ���...

	// Update Velocity
	m_Velocity.direction += m_Acceleration;
	m_Velocity.direction.Normalize();	

}

DirectX::SimpleMath::Vector3 Boid::Align()
{
	/// SimpleMath ���ӽ����̽� ��� 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////
	 
	// ����(����) �˻��� ���� => �ݰ�
	float perceptionRadius = 15.0f;

	// �ݰ� ���� �����ϴ� boid �� ��
	float total = 0.0f;

	// ���׷�? Sterring
	Vector3 steering = Vector3::Zero;

	// ���̵� ��ü���� ��ȸ�ϸ鼭
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			 this->m_Pos.x,  this->m_Pos.y,  this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// �ӵ��� �� ���ϰ�
			steering += other->m_Velocity.direction;

			// ���̵� ���� ����
			total++;
		}
		
	}

	if (total > 0)
	{
		// ������ ��ŭ ����
		steering /= total;

		// ���� �������� �ٷ� ���� ���̾ƴ϶� ������ ������ ���Ͽ�
		steering -= this->m_Velocity.direction;
	}
	
	return steering;
}

DirectX::SimpleMath::Vector3 Boid::Cohesion()
{
	/// SimpleMath ���ӽ����̽� ��� 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////

	// ����(����) �˻��� ���� => �ݰ�
	float perceptionRadius = 30.0f;
	
	// �ݰ� ���� �����ϴ� boid �� ��
	float total = 0.0f;

	// ���׷�? Sterring
	Vector3 steering = Vector3::Zero;


	// ���̵� ��ü���� ��ȸ�ϸ鼭
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			this->m_Pos.x, this->m_Pos.y, this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// ��ġ�� ���Ѵ�
			steering += other->m_Pos;

			// ���̵� ���� ����
			total++;
		}

	}

	if (total > 0)
	{
		// ������ ��ŭ ����
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
	/// SimpleMath ���ӽ����̽� ��� 
	using namespace DirectX::SimpleMath;
	///////////////////////////////////////

	// ����(����) �˻��� ���� => �ݰ�
	float perceptionRadius = 15.0f;

	// �ݰ� ���� �����ϴ� boid �� ��
	float total = 0.0f;

	// ���׷�? Sterring
	Vector3 steering = Vector3::Zero;

	// ���̵� ��ü���� ��ȸ�ϸ鼭
	for (auto& other : *m_vecFlock)
	{
		float d = CommonMacro::GetDistance(
			this->m_Pos.x, this->m_Pos.y, this->m_Pos.y,
			other->m_Pos.x, other->m_Pos.y, other->m_Pos.z
		);

		if (other != this && d < perceptionRadius)
		{
			// �ݴ�� �̵��ϰ� ���� ���͸� ����.
			Vector3 diff = this->m_Pos - other->m_Pos;	// other �ʿ��� this�� ���ϴ� ���� 
			diff.Normalize();

			/// �� �𸣰ڴ�... �и�....
			// �Ÿ��� �ݺ���ϵ���
			//diff /= (d);		// �Ÿ��� �ּ��� ũ�Ⱑ �۾���.
			diff = (diff * d);

			steering += diff;

			// ���̵� ���� ����
			total++;
		}

	}

	if (total > 0)
	{
		// ������ ��ŭ ����
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
	// �ùķ��̼� �ϸ鼭 ���� ���� �ȿ� ���ֳ��� ���� ���� ���� ��ġ

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
