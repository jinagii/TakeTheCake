#pragma once

/// <summary>
/// CupCake Class
/// Writer : YoKing
/// 
/// 플레이어가 목표로 하는 CupCake 클래스
/// 
/// 별다른 기능은 없지만 플레이어가 컵케익을 얻으면
/// 플레이어를 따라가야 한다.
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
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	void CheckCollision();								//  충돌 체크

protected:

private:
	class IObjectBase* m_pTarget;	// 타겟 플레이어

	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;

	bool m_IsFollowingMode;

};

