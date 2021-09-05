/// <summary>
/// Flock Class
/// Writer : YoKing
/// 
/// ��ü���� �ڿ������� �������� �ൿ�ϴ� ����� ������ ����
/// (�� ��, ����� ����, �δ� �̵�)
/// 
/// Flocking = ����
/// Boid	 = ���� �� �ϳ��ϳ��� ��ü
/// 
/// FLock ��ü�� Boid ���� ���� ��ü ������ �����Ѵ�.
/// 
/// First Create	  : 2021.08.12
/// Last Modification : 2021.08.12
/// </summary>

#pragma once

#include "IObjectBase.h"

// ������Ʈ ����Ʈ(�Ŵ���)�� �߰��ϰ� �����ϱ� ���� IObjectBase �� ���
//template <typename T> // ���ø��� �õ��ߴµ� ������ �κ��� ����.. ���߿� ��������...
class Flock : public IObjectBase
{
public:
	Flock();
	virtual ~Flock();
	
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	std::vector<class Boid*>& GetBoidList();
	void SetTarget(IObjectBase* pTarget);
	void AddBoidObject();

	// Flock �ӽ� �׽�Ʈ ������
	static bool isRunning;

	class Camera* m_pCamera;

protected:


private:
	/// Flock �� ���� Boid ��ü ����
	// ������ ���ԵǴ� ������ ��ü��...
	std::vector<class Boid*> m_vecBoids;

	/// DX ���� ������Ʈ
	IDXObjectBase* m_pMesh;	// �޽�
							// �ϴ� Initialize() ���� �ð� �ϱ� ����..
							// Flock ������ �� Mesh �� �ް� Boid�� �߰��� �� �������ش�.

	/// �� ������ ���� Target �� �ʿ��Ұ� ����..
	IObjectBase* m_pTarget;

};

