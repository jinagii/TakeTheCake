#pragma once

/// <summary>
/// Trap Class
/// Writer : YoKing
/// 
/// ���̵��� ���ֵΰ� �ִ� ��� Ŭ����
/// Ʈ������ ��ȣ�ۿ��ϸ� ���� ���̸� �����Ѵ�
/// 
/// 
/// First Create	  : 2021.08.25
/// Last Modification : 2021.08.25
/// </summary>

#include "IObjectBase.h"

class Trap : public IObjectBase
{
public:
	Trap(class IDXObjectBase* pMesh);
	~Trap();

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

	/// Ʈ���� ���ΰ� �ִ� ����
	class IDXObjectBase* m_Ant;
	bool m_bAnt;	// ���̰� �ֳ� ����

};