#pragma once

#include "IObjectBase.h"

/// <summary>
/// Boid Class
/// Writer : YoKing
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
/// Last Modification : 2021.08.15
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

class Boid : public IObjectBase
{
public:
	Boid();
	virtual ~Boid();

	virtual void Initialize() override;
	virtual void Release()	override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	void SetFlock(std::vector<Boid*>* pFlock);
	void SetTarget(IObjectBase* pTarget);

	/// Three Rule Of Boid
	void Flock();
	DirectX::SimpleMath::Vector3 Align();		// ����
	DirectX::SimpleMath::Vector3 Cohesion();	// ����
	DirectX::SimpleMath::Vector3 Separation();	// �и�

	float GetMaxSpeed() const;
	void SetMaxSpeed(const float speed);

	// ��������?
	// �� ������� �Ÿ�?
	// �ټ�?
	// ���� Ư�� (����, ����)
	// Add-Hoc

protected:
	/// ���̵� ��ü�� ���� ���� ����
	std::vector<Boid*>* m_vecFlock;

	/// ���̵� ��ü�� ���� Target
	IObjectBase* m_pTarget;

	float m_MaxSpeed;

private:
	// �׽�Ʈ �ùķ��̼ǿ� ��ǥ ���α�
	void TrappingPos();
};

