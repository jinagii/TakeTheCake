#pragma once

/// <summary>
/// Fire Class
/// Writer : YoKing
/// 
/// 전자렌지 위에서 플레이어의 이동을 방해하는 기믹 Fire 클래스
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
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	void CheckCollision();								//  충돌 체크

	void FireSwitch();

	/// DX 엔진 오브젝트
	class IDXObjectBase* m_pFireOff;	// 매쉬
										// [0] -> Off
										// [1] -> On

protected:

private:
	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;

	// 플레이어 타겟
	class IObjectBase* m_pTarget;	// 타겟 플레이어

	bool  m_bFireOn;				// 불 모드 On/Off
	float m_OnDurationTime;			// 불  On 일 때 지속시간
	float m_OffDurationTime;		// 불 Off 일 때 지속시간
	float m_Timer;					// 타이머
};

