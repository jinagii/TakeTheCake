#include "pch.h"

// 엔진
#include "IDXObjectBase.h"
#include "DXObjectFactory.h"
#include "Flock.h"
#include "ObjectManager.h"
#include "AntBase.h"

AntBase::AntBase(class IDXObjectBase* pMesh)
	: Boid()
	, m_pMesh(nullptr)
{
	/// 메쉬 세팅
	SetMesh(pMesh);

	/// 기타 초기화
	Initialize();
}

AntBase::~AntBase()
{
	this->Release();
}

void AntBase::Initialize()
{
	IObjectBase::Initialize();
	Boid::Initialize();
}

void AntBase::Release()
{
	SAFE_DELETE(m_pMesh);
}

void AntBase::Update(Camera* pCamera, float dTime)
{
	if (Flock::isRunning == true)
	{
		Boid::Update(pCamera, dTime);
	}

	UpdateMesh(pCamera, dTime);
}

void AntBase::Render()
{
	/// Render Mesh
	m_pMesh->Render();
}

void AntBase::UpdateMesh(Camera* pCamera, float dTime)
{
	/////////////////////////////////////////////
	// Update Mesh
	float offsetX = m_Pos.x;
	float offsetY = m_Pos.y;
	float offsetZ = m_Pos.z;

	Matrix matrix;
	matrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);

	m_pMesh->SetTransTM(matrix);

	m_pMesh->Update(pCamera, dTime / ObjectManager::sm_ObjectPoolSize);	// 애니메이션 속도 버그를 해결하기 위해 dTime 값을 무리의 사이즈 만큼 나눠 보정해준다.
}

