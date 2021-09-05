#pragma once

/// <summary>
/// Boid Class
/// Writer : 최 요 환
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
/// Last Modification : 2021.08.05 
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

#include "IDXObjectBase.h"

// 참고하는 영상을 최대한 따라가기 위하여 Velocity 구조체를 만듦
// velocity(direction) and magnitude
struct Velocity
{
	Velocity(DirectX::SimpleMath::Vector3 vec3, float magnitude)
	{
		direction = vec3;
		this->magnitude = magnitude;
	}

	DirectX::SimpleMath::Vector3 direction;	// 속도
	float magnitude;						// 크기
};

class Boid_Old : public IDXObjectBase
{
public:
	Boid_Old(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, class DXObjectFactory* pFactory);
	~Boid_Old();

	void Initialize();
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render();

protected:
	/// <summary>
	/// 일단 새벽에 억지로 끼웜맞춰본 구조...
	/// 게임 오브젝트 Boid가 IDXObjectBase* 의 MeshObject를 들고 있는 구조...
	/// 일단 띄워보자;;;
	/// 
	/// 본체 오브젝트(Boid 등)의 트랜스폼이 변환되면 mesh도 그에 맞추어 변환시켜줘야 할텐데??
	/// </summary>
	class IDXObjectBase* m_MeshObject;

private:
	/// position / velocity / acceleration 필요
	DirectX::SimpleMath::Vector3 m_Pos;
	Velocity m_Velocity;
	DirectX::SimpleMath::Vector3 m_Acceleration;


};
