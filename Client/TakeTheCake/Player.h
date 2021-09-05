#pragma once

/// <summary>
/// Player Class
/// Writer : YoKing
/// 
/// �÷��̾ ����(��Ʈ��)�ϴ� ��ü�� Ŭ����
/// 
/// ����, ����, ȸ�� ���� �⺻ �̵��� ��.
/// �޽��� ������.
/// �������̵��� �÷��̾ Ÿ������ ���󰣴�.
/// 
/// First Create	  : 2021.08.15
/// Last Modification : 2021.08.18
/// </summary>

#include "IObjectBase.h"

class Player : public IObjectBase
{
public:
	Player(class IDXObjectBase* pMesh);
	~Player();

	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	

	int GetHp() const;
	void SetHp(const int hp);
	bool GetPowerOverWhelming() const;

	float GetMovePower();
	float GetRotatePower();

	bool IsDead() const;

	void Move(class Camera* pCamera, const float dTime);	// �÷��̾� �̵�
	void MoveForward(const float dTime, const float dir = 1.0f);	// dir -> 1 �̸� ����
															// dir -> 2 �̸� ����
	void Rotate(const float dTime);
	
	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// �浹üũ�� �浹ü �߰�
	void Hit();	// �� ���� ������Ʈ�� �浹 üũ

	void OnPickCakeUp();	// ����ũ�� ����� ��
	void OnStandBy();		// �Ϲ� ���·� ��ȯ �Ǿ��� ��

	void SetNavigationMesh(class ClNavigation* navigation);

	void SetCameraZoom(SHORT zoomDelta);

private:
	/// ���� �ܺο��� �޾ƿ����� ����
	Vector3 m_EyePosW;

	/// �浹üũ�� debugCircle ����
	std::vector<class DebugCircle*> m_DebugCircles;

	float m_dTime;

	/// hp 
	int m_HpMax;	// �ִ� Hp
	int m_Hp;		// 0�̵Ǹ� ���ӿ��� ó��

	bool m_IsDead;;		// ��� ����


	bool m_bPowerOverWhelming;		// ���� ����
	float m_Timer;					// ���� Ÿ�� ���� Ÿ�̸�
	float m_BlinkTimer;				// ������ Ÿ�̸�
	bool m_bVisible;				// ���� ����

	/// ���� �ִ� ����Ž��� �޾ƿ��� �뵵
	class ClNavigation* m_pNavigation;

	float m_MovePower;
	float m_RotatePower;

	/// Sounds
	class Sound* m_sndMove;
	class Sound* m_sndDie;

	// ī�޶� ����ٴϰ� �Ǵ� ������.
	// �̰��� �����ϸ� �� �� �ƿ��� �ȴ�.
	Vector3 m_OffsetVec_Camera;

public:
	Vector3 GetOffsetVec();

private:
	float m_CameraZoom;
	float m_CameraYOffset;
};