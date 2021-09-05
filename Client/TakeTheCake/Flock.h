/// <summary>
/// Flock Class
/// Writer : YoKing
/// 
/// 개체들이 자연스럽게 무리지어 행동하는 모습을 구현해 보자
/// (새 떼, 물고기 무리, 부대 이동)
/// 
/// Flocking = 무리
/// Boid	 = 무리 속 하나하나의 개체
/// 
/// FLock 개체가 Boid 들을 여러 개체 가지고 관리한다.
/// 
/// First Create	  : 2021.08.12
/// Last Modification : 2021.08.12
/// </summary>

#pragma once

#include "IObjectBase.h"

// 오브젝트 리스트(매니저)에 추가하고 관리하기 위해 IObjectBase 를 상속
//template <typename T> // 템플릿을 시도했는데 막히는 부분이 생김.. 나중에 개선하자...
class Flock : public IObjectBase
{
public:
	Flock();
	virtual ~Flock();
	
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update(class Camera* pCamera, float dTime) override;
	virtual void Render() override;

	std::vector<class Boid*>& GetBoidList();
	void SetTarget(IObjectBase* pTarget);
	void AddBoidObject();

	// Flock 임시 테스트 조절용
	static bool isRunning;

	class Camera* m_pCamera;

protected:


private:
	/// Flock 이 가질 Boid 개체 벡터
	// 무리에 포함되는 각각의 개체들...
	std::vector<class Boid*> m_vecBoids;

	/// DX 엔진 오브젝트
	IDXObjectBase* m_pMesh;	// 메쉬
							// 일단 Initialize() 명세에 맡게 하기 위해..
							// Flock 생성할 때 Mesh 를 받고 Boid를 추가할 때 전달해준다.

	/// 이 무리가 향할 Target 이 필요할거 같다..
	IObjectBase* m_pTarget;

};

