#pragma once

/// <summary>
/// Boid Class
/// Writer : �� �� ȯ
/// 
/// ��ü���� �ڿ������� �������� �ൿ�ϴ� ����� ������ ����
/// (�� ��, ����� ����, �δ� �̵�)
/// 
/// �ϴ� ������
/// 
/// Flocking = ����
/// Boid	 = ���� �� �ϳ��ϳ��� ��ü
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.05 
/// </summary>


/// <summary>
/// Boid ���� �ϳ��ϳ��� �������� ��ü��
/// �⺻������ ������Ʈ�� ������ �־�� �� �ɷ�(�׼�)���� �ʿ�
/// Initialize, Update, Render(������ ����� ��)
/// �޽�, �ݶ��̴�
/// ��ǥ, �̵�, �浹üũ
/// 
/// SimpleMath ������ � ����Ѵٰ� �Ѵ�.
/// 
/// ����(������ �ϴ� �� ������ �ڵ带 ���� ���鼭 ����� ��..)
/// https://www.youtube.com/watch?v=mhjuuHl6qHM
/// </summary>

#include "IDXObjectBase.h"

// �����ϴ� ������ �ִ��� ���󰡱� ���Ͽ� Velocity ����ü�� ����
// velocity(direction) and magnitude
struct Velocity
{
	Velocity(DirectX::SimpleMath::Vector3 vec3, float magnitude)
	{
		direction = vec3;
		this->magnitude = magnitude;
	}

	DirectX::SimpleMath::Vector3 direction;	// �ӵ�
	float magnitude;						// ũ��
};

class Boid_Old : public IDXObjectBase
{
public:
	Boid_Old(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, class DXObjectFactory* pFactory);
	~Boid_Old();

	void Initialize();
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render();

protected:
	/// <summary>
	/// �ϴ� ������ ������ �������纻 ����...
	/// ���� ������Ʈ Boid�� IDXObjectBase* �� MeshObject�� ��� �ִ� ����...
	/// �ϴ� �������;;;
	/// 
	/// ��ü ������Ʈ(Boid ��)�� Ʈ�������� ��ȯ�Ǹ� mesh�� �׿� ���߾� ��ȯ������� ���ٵ�??
	/// </summary>
	class IDXObjectBase* m_MeshObject;

private:
	/// position / velocity / acceleration �ʿ�
	DirectX::SimpleMath::Vector3 m_Pos;
	Velocity m_Velocity;
	DirectX::SimpleMath::Vector3 m_Acceleration;


};
