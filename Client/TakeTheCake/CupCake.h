#pragma once

/// <summary>
/// CupCake Class
/// Writer : YoKing
/// 
/// �÷��̾ ��ǥ�� �ϴ� CupCake Ŭ����
/// 
/// ���ٸ� ����� ������ �÷��̾ �������� ������
/// �÷��̾ ���󰡾� �Ѵ�.
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23
/// </summary>

#include "IObjectBase.h"


class CupCake : public IObjectBase
{
public:
	CupCake(class IDXObjectBase* pMesh);
	~CupCake();

	virtual void Initialize() override;
	virtual void Release()	override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;

	void UpdateMesh(class Camera* pCamera, float dTime);


	void SetTarget(IObjectBase* pTarget);

	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// �浹üũ�� �浹ü �߰�
	void CheckCollision();								//  �浹 üũ

protected:

private:
	class IObjectBase* m_pTarget;	// Ÿ�� �÷��̾�

	/// �浹üũ�� debugCircle ����
	std::vector<class DebugCircle*> m_DebugCircles;

	bool m_IsFollowingMode;

};

