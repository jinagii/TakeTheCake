#include "pch.h"
#include "DXObjectFactory.h"
#include "AntBase.h"
#include "ObjectManager.h"
#include "Flock.h"

bool Flock::isRunning = false;

Flock::Flock()
	: m_pMesh(nullptr)
	, m_pTarget(nullptr)
{
}

Flock::~Flock()
{
	Release();
}

void Flock::Initialize()
{
	isRunning = false;

	/// ���� ����� ���� ������ �ִٸ� ���� ������ ���ش�.
	Release();

	/// boid �߰�
	for (int i = 0; i < 10; i++)
	{
		AddBoidObject();

		ObjectManager::sm_ObjectPoolSize++;	// ���̵� �߰��� ��ü ������Ʈ Ǯ�� ������ 1 ����
	}

	//m_vecBoids[0]->SetPos({ -5.0f, 0.0f, 0.0f });
	//m_vecBoids[0]->SetDirection({ 1.0f, 0.0f, 0.5f });
	//
	//
	//m_vecBoids[1]->SetPos({ 5.0f, 0.0f, 0.0f });
	//m_vecBoids[1]->SetDirection({ -1.0f, 0.0f, 0.5f });
}

void Flock::Release()
{
	for (auto& boid : m_vecBoids)
	{
		ReleaseCOM(boid);
		SAFE_DELETE(boid);
	}

	SAFE_DELETE(m_pTarget);
	SAFE_DELETE(m_pMesh);
}

void Flock::Update(class Camera* pCamera, float dTime)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('Z'))
	{
		isRunning = !isRunning;
	}

	// Flock �˰��� ����
	for (auto boid : m_vecBoids)
	{
		boid->Flock();
	}

	// ������ ��� ��ü���� ������Ʈ
	for (auto boid : m_vecBoids)
	{
		boid->Update(pCamera, dTime);	
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('F'))
	{
		AddBoidObject();
	}
}

void Flock::Render()
{
	// ������ ��� ��ü���� ����
	for (auto boid : m_vecBoids)
	{
		boid->Render();
	}
}

std::vector<class Boid*>& Flock::GetBoidList()
{
	return m_vecBoids;
}

void Flock::SetTarget(IObjectBase* pTarget)
{
	// nullptr üũ
	if (!pTarget) return;

	// ������ Ÿ���� �����ϰ�
	this->m_pTarget = pTarget;

	// ������ �� boid ��ü���� Ÿ�ٵ� �缳��
	for(auto boid : m_vecBoids)
	{
		boid->SetTarget(pTarget);

		AntBase* ant = dynamic_cast<AntBase*>(boid);
		if (ant != nullptr)
		{
			//ant->UpdateMesh();
		}
	}

}

void Flock::AddBoidObject()
{
	/// Ant ���� �� �ʱ�ȭ
	Boid* ant = dynamic_cast<Boid*>( ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::ANT) );
	ant->SetFlock(&this->m_vecBoids);
	ant->SetTarget(this->m_pTarget);
	
	//dynamic_cast<AntBase*>(ant)->UpdateMesh();

	// ������ ant �߰�
	m_vecBoids.push_back(ant);
}
