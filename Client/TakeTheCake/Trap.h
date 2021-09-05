#pragma once

/// <summary>
/// Trap Class
/// Writer : YoKing
/// 
/// 개미들을 가둬두고 있는 기믹 클래스
/// 트랩에서 상호작용하면 갇힌 개미를 구출한다
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
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	void CheckCollision();								//  충돌 체크


protected:

private:
	class IObjectBase* m_pTarget;	// 타겟 플레이어

	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;

	/// 트랩이 가두고 있는 개미
	class IDXObjectBase* m_Ant;
	bool m_bAnt;	// 개미가 있나 없나

};