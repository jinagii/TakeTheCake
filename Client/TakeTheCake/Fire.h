#pragma once

/// <summary>
/// Fire Class
/// Writer : YoKing
/// 
/// ���ڷ��� ������ �÷��̾��� �̵��� �����ϴ� ��� Fire Ŭ����
/// 
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23
/// </summary>

#include "IObjectBase.h"


class Fire : public IObjectBase
{
public:
	Fire(class IDXObjectBase* pMeshOff, class IDXObjectBase* pMeshOn);
	~Fire();

	virtual void Initialize() override;
	virtual void Release()	override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;

	void UpdateMesh(class Camera* pCamera, float dTime);

	void SetTarget(IObjectBase* pTarget);

	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// �浹üũ�� �浹ü �߰�
	void CheckCollision();								//  �浹 üũ

	void FireSwitch();

	/// DX ���� ������Ʈ
	class IDXObjectBase* m_pFireOff;	// �Ž�
										// [0] -> Off
										// [1] -> On

protected:

private:
	/// �浹üũ�� debugCircle ����
	std::vector<class DebugCircle*> m_DebugCircles;

	// �÷��̾� Ÿ��
	class IObjectBase* m_pTarget;	// Ÿ�� �÷��̾�

	bool  m_bFireOn;				// �� ��� On/Off
	float m_OnDurationTime;			// ��  On �� �� ���ӽð�
	float m_OffDurationTime;		// �� Off �� �� ���ӽð�
	float m_Timer;					// Ÿ�̸�
};

