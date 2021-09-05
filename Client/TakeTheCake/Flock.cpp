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

	/// 새로 만들기 전에 이전게 있다면 먼저 릴리즈 해준다.
	Release();

	/// boid 추가
	for (int i = 0; i < 10; i++)
	{
		AddBoidObject();

		ObjectManager::sm_ObjectPoolSize++;	// 보이드 추가후 전체 오브젝트 풀의 사이즈 1 증가
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

	// Flock 알고리즘 실행
	for (auto boid : m_vecBoids)
	{
		boid->Flock();
	}

	// 무리의 모든 개체들을 업데이트
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
	// 무리의 모든 개체들을 렌더
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
	// nullptr 체크
	if (!pTarget) return;

	// 무리의 타겟을 설정하고
	this->m_pTarget = pTarget;

	// 무리의 각 boid 객체들의 타겟도 재설정
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
	/// Ant 생성 및 초기화
	Boid* ant = dynamic_cast<Boid*>( ObjectFactory::GetInstance()->CreateObject(OBJECT_TYPE::ANT) );
	ant->SetFlock(&this->m_vecBoids);
	ant->SetTarget(this->m_pTarget);
	
	//dynamic_cast<AntBase*>(ant)->UpdateMesh();

	// 무리에 ant 추가
	m_vecBoids.push_back(ant);
}
