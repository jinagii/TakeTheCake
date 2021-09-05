#include "pch.h"

/// 임시 팩토리 포함
#include "DXObjectFactory.h"

///

#include "Boid_Old.h"

Boid_Old::Boid_Old(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DXObjectFactory* pFactory)
	: IDXObjectBase(pDevice, pDeviceContext)
	, m_Velocity(DirectX::SimpleMath::Vector3::Zero, 0.0f)	// 변수 초기화 워닝 생겨서 만들어 봤는데 해결 안됌...
{
	/// 메쉬 넣기
	//m_MeshObject = pFactory->CreateObjectFromASEParser("CupCake");

	/// positioni / velocity / acceleration 초기화
	m_Pos = DirectX::SimpleMath::Vector3::Zero;

	m_Velocity.direction = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f);	// y 성분은 사용하지 않는다. x, z 적당히 세팅
	m_Velocity.magnitude = 1.0f;

	m_Acceleration = DirectX::SimpleMath::Vector3::Zero;

}

Boid_Old::~Boid_Old()
{
}

void Boid_Old::Initialize()
{
}

void Boid_Old::Update(Camera* pCamera, float dTime)
{
	/// 본체인 Boid가 업데이트 된다(트랜스폼의 변화가 있다면)
	/// 그에 맞추어 메쉬도 업데이트 해줘야 한다... (Boid가 움직이는데 매쉬만 놓고 움직이면 안되지..)
	/// 근데 어떻게 또 끼워 맞추지...?

	// Update Pos
	m_Pos += m_Velocity.direction;

	// 업데이트 가 될 때 m_Pos가 갱신되는 것은 확인!
	//std::cout << m_Pos.x << " | " << m_Pos.y << " | " << m_Pos.z << std::endl;

	// Update Velocity
	m_Velocity.direction += m_Acceleration;

	// 메쉬 업데이트
	m_MeshObject->Update(pCamera, dTime);

}

void Boid_Old::Render()
{
	// 메쉬 렌더
	m_MeshObject->Render();
}
