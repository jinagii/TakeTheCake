#pragma once

/// <summary>
/// Player Class
/// Writer : YoKing
/// 
/// 플레이어가 조작(컨트롤)하는 개체의 클래스
/// 
/// 전진, 후진, 회전 등의 기본 이동을 함.
/// 메쉬를 가진다.
/// 무리개미들이 플레이어를 타겟으로 따라간다.
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

	void Move(class Camera* pCamera, const float dTime);	// 플레이어 이동
	void MoveForward(const float dTime, const float dir = 1.0f);	// dir -> 1 이면 전진
															// dir -> 2 이면 후진
	void Rotate(const float dTime);
	
	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	void Hit();	// 씬 내의 오브젝트들 충돌 체크

	void OnPickCakeUp();	// 케이크를 들었을 때
	void OnStandBy();		// 일반 상태로 전환 되었을 때

	void SetNavigationMesh(class ClNavigation* navigation);

	void SetCameraZoom(SHORT zoomDelta);

private:
	/// 빛은 외부에서 받아오도록 하자
	Vector3 m_EyePosW;

	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;

	float m_dTime;

	/// hp 
	int m_HpMax;	// 최대 Hp
	int m_Hp;		// 0이되면 게임오버 처리

	bool m_IsDead;;		// 사망 여부


	bool m_bPowerOverWhelming;		// 무적 변수
	float m_Timer;					// 무적 타임 해제 타이머
	float m_BlinkTimer;				// 깜빡이 타이머
	bool m_bVisible;				// 렌더 유무

	/// 씬에 있는 내비매쉬를 받아오는 용도
	class ClNavigation* m_pNavigation;

	float m_MovePower;
	float m_RotatePower;

	/// Sounds
	class Sound* m_sndMove;
	class Sound* m_sndDie;

	// 카메라를 따라다니게 되는 오프셋.
	// 이것을 조절하면 줌 인 아웃이 된다.
	Vector3 m_OffsetVec_Camera;

public:
	Vector3 GetOffsetVec();

private:
	float m_CameraZoom;
	float m_CameraYOffset;
};