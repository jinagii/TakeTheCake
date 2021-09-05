#pragma once

#include "IObjectBase.h"

/// <summary>
/// Player Class
/// Writer : YoKing/LeHide
/// 
/// 플레이어 개미를 따라다니는 개체의 클래스
/// 
/// 전진, 후진, 회전 등의 기본 이동을 함.
/// 메쉬를 가진다.
/// 플레이어를 타겟으로 따라간다.
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


	// 특정 타겟(플레이어)를 따라다닌다.
	void Move(class Player* target, const float dTime);

	// 방향을 설정해준다.
	void Rotate(const float dTime);

	void SetColliderType();
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	
	void SetNavigationMesh(class ClNavigation* navigation);

private:

	class Player* m_pTarget;

	Vector3 m_RandomOffset;		// 따라다니기를 할 때, 랜덤하게 배치
	Vector3 m_DirVec;

	/// 빛은 외부에서 받아오도록 하자
	Vector3 m_EyePosW;

	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;

	/// 씬에 있는 내비매쉬를 받아오는 용도
	class ClNavigation* m_pNavigation;

	float m_MovePower;
	float m_RotatePower;

	/// Sounds
	class Sound* m_sndMove;
	class Sound* m_sndDie;

};

