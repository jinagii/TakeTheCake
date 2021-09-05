#pragma once

/// <summary>
/// Knife Class
/// Writer : YoKing
/// 
/// ȸ���ϸ鼭 �÷��̾��� ���� ���� ������Ʈ
/// Knife�� �浹�ϸ� ü���� ���δ�
/// 
/// 
/// First Create	  : 2021.08.23
/// Last Modification : 2021.08.23
/// </summary>

#include "IObjectBase.h"

class Knife : public IObjectBase
{
public:
	Knife(class IDXObjectBase* pMesh);
	~Knife();

	virtual void Initialize() override;
	virtual void Release()	override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;


	void UpdateMesh(class Camera* pCamera, float dTime);

	void SetTarget(IObjectBase* pTarget);

	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// �浹üũ�� �浹ü �߰�
	void CheckCollision();								//  �浹 üũ

	void Rotate(const float dTime);

protected:

private:
	class IObjectBase* m_pTarget;	// Ÿ�� �÷��̾�

	/// �浹üũ�� debugCircle ����
	std::vector<class DebugCircle*> m_DebugCircles;
};

