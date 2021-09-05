#pragma once

/// <summary>
/// Knife Class
/// Writer : YoKing
/// 
/// 회전하면서 플레이어의 길을 막는 오브젝트
/// Knife에 충돌하면 체력이 깎인다
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
	void AddCollider(class DebugCircle* collider);		// 충돌체크할 충돌체 추가
	void CheckCollision();								//  충돌 체크

	void Rotate(const float dTime);

protected:

private:
	class IObjectBase* m_pTarget;	// 타겟 플레이어

	/// 충돌체크용 debugCircle 벡터
	std::vector<class DebugCircle*> m_DebugCircles;
};

