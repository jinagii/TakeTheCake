#pragma once

#include "IObjectBase.h"

/// <summary>
/// Boid Class
/// Writer : YoKing
/// 
/// 개체들이 자연스럽게 무리지어 행동하는 모습을 구현해 보자
/// (새 떼, 물고기 무리, 부대 이동)
/// 
/// 일단 만들어보자
/// 
/// Flocking = 무리
/// Boid	 = 무리 속 하나하나의 개체
/// 
/// First Create	  : 2021.08.04
/// Last Modification : 2021.08.15
/// </summary>


/// <summary>
/// Boid 들은 하나하나의 독립적인 개체들
/// 기본적으로 오브젝트가 가지고 있어야 할 능력(액션)들이 필요
/// Initialize, Update, Render(렌더의 대상이 됌)
/// 메쉬, 콜라이더
/// 좌표, 이동, 충돌체크
/// 
/// SimpleMath 정도는 까서 사용한다고 한다.
/// 
/// 참고(많지만 일단 이 영상의 코드를 많이 보면서 만드는 중..)
/// https://www.youtube.com/watch?v=mhjuuHl6qHM
/// </summary>

class Boid : public IObjectBase
{
public:
	Boid();
	virtual ~Boid();

	virtual void Initialize() override;
	virtual void Release()	override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	void SetFlock(std::vector<Boid*>* pFlock);
	void SetTarget(IObjectBase* pTarget);

	/// Three Rule Of Boid
	void Flock();
	DirectX::SimpleMath::Vector3 Align();		// 정렬
	DirectX::SimpleMath::Vector3 Cohesion();	// 응집
	DirectX::SimpleMath::Vector3 Separation();	// 분리

	float GetMaxSpeed() const;
	void SetMaxSpeed(const float speed);

	// 리더여부?
	// 옆 동료와의 거리?
	// 텐션?
	// 물리 특성 (공중, 해저)
	// Add-Hoc

protected:
	/// 보이드 개체가 속한 무리 정보
	std::vector<Boid*>* m_vecFlock;

	/// 보이드 개체가 향할 Target
	IObjectBase* m_pTarget;

	float m_MaxSpeed;

private:
	// 테스트 시뮬레이션용 좌표 가두기
	void TrappingPos();
};

