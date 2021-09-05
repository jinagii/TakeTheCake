#pragma once

#include "IObjectBase.h"

/// <summary>
/// Player Class
/// Writer : YoKing/LeHide
/// 
/// �÷��̾� ���̸� ����ٴϴ� ��ü�� Ŭ����
/// 
/// ����, ����, ȸ�� ���� �⺻ �̵��� ��.
/// �޽��� ������.
/// �÷��̾ Ÿ������ ���󰣴�.
/// 
/// First Create	  : 2021.08.27
/// Last Modification : 2021.08.27
/// </summary>
class Follower : public IObjectBase
{
public:
	Follower(class IDXObjectBase* pMesh);
	~Follower();

	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;


	void SetTarget(class Player* pPlayer);

	float GetMovePower();
	float GetRotatePower();


	// Ư�� Ÿ��(�÷��̾�)�� ����ٴѴ�.
	void Move(class Player* target, const float dTime);

	// ������ �������ش�.
	void Rotate(const float dTime);

	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// �浹üũ�� �浹ü �߰�
	
	void SetNavigationMesh(class ClNavigation* navigation);

private:

	class Player* m_pTarget;

	Vector3 m_RandomOffset;		// ����ٴϱ⸦ �� ��, �����ϰ� ��ġ
	Vector3 m_DirVec;

	/// ���� �ܺο��� �޾ƿ����� ����
	Vector3 m_EyePosW;

	/// �浹üũ�� debugCircle ����
	std::vector<class DebugCircle*> m_DebugCircles;

	/// ���� �ִ� ����Ž��� �޾ƿ��� �뵵
	class ClNavigation* m_pNavigation;

	float m_MovePower;
	float m_RotatePower;

	/// Sounds
	class Sound* m_sndMove;
	class Sound* m_sndDie;

};

