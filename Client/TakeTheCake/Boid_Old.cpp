#include "pch.h"

/// �ӽ� ���丮 ����
#include "DXObjectFactory.h"

///

#include "Boid_Old.h"

Boid_Old::Boid_Old(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DXObjectFactory* pFactory)
	: IDXObjectBase(pDevice, pDeviceContext)
	, m_Velocity(DirectX::SimpleMath::Vector3::Zero, 0.0f)	// ���� �ʱ�ȭ ���� ���ܼ� ����� �ôµ� �ذ� �ȉ�...
{
	/// �޽� �ֱ�
	//m_MeshObject = pFactory->CreateObjectFromASEParser("CupCake");

	/// positioni / velocity / acceleration �ʱ�ȭ
	m_Pos = DirectX::SimpleMath::Vector3::Zero;

	m_Velocity.direction = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f);	// y ������ ������� �ʴ´�. x, z ������ ����
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
	/// ��ü�� Boid�� ������Ʈ �ȴ�(Ʈ�������� ��ȭ�� �ִٸ�)
	/// �׿� ���߾� �޽��� ������Ʈ ����� �Ѵ�... (Boid�� �����̴µ� �Ž��� ���� �����̸� �ȵ���..)
	/// �ٵ� ��� �� ���� ������...?

	// Update Pos
	m_Pos += m_Velocity.direction;

	// ������Ʈ �� �� �� m_Pos�� ���ŵǴ� ���� Ȯ��!
	//std::cout << m_Pos.x << " | " << m_Pos.y << " | " << m_Pos.z << std::endl;

	// Update Velocity
	m_Velocity.direction += m_Acceleration;

	// �޽� ������Ʈ
	m_MeshObject->Update(pCamera, dTime);

}

void Boid_Old::Render()
{
	// �޽� ����
	m_MeshObject->Render();
}
