/// <summary>
/// IObjectBase Class
/// Writer : 최 요 환
/// 
/// 오브젝트들의 기본이 되는 클래스
/// 
/// 추상클래스로 만들고 하부 클래스들이 상속해서 함수를 구현하게 만든다.
/// 
/// First Create	  : 2021.08.10
/// Last Modification : 2021.08.10 
/// </summary>
/// 
#pragma once

enum class COLLISION_OBJECT_TYPE
{
	None,
	Player,
	CupCake,
	Fire,
	Knife,
	Trap
};

// 참고하는 영상을 최대한 따라가기 위하여 Velocity 구조체를 만듦
// velocity(direction) and magnitude
struct Velocity
{
	Velocity() 
		: direction()
		, magnitude()
	{}

	Velocity(DirectX::SimpleMath::Vector3 vec3, float magnitude)
	{
		direction = vec3;
		this->magnitude = magnitude;
	}

	DirectX::SimpleMath::Vector3 direction;	// 속도
	float magnitude;						// 크기
};

class IObjectBase abstract
{
public:
	IObjectBase();	
	virtual ~IObjectBase() {}

	virtual void Initialize() { m_Pos = m_StartPos; SetTransTM(); }
	virtual void Release() {}
	virtual void Update(class Camera* pCamera, float dTime) {}
	virtual void Render() {}

/// <summary>
/// ////////////////////////////////////////////////////////////////
/// </summary>
	float GetMagnitude() const;
	void SetMagnitude(const float val);

	DirectX::SimpleMath::Vector3 GetPos() const;
	void SetPos(const Vector3 pos);
	void SetStartPos(const Vector3 pos);

	DirectX::SimpleMath::Vector3 GetDirection() const;
	void SetDirection(Vector3 dir);

	float GetAngle() const;

	Matrix GetTransTM() const;
	void SetTransTM();
	void SetTransTM(Matrix matrix);

	/// 확장성을 위해... 나중에 메쉬를 변경해야할 일이 생긴다면...
	virtual IDXObjectBase* GetMesh() const;
	virtual void SetMesh(class IDXObjectBase* pMesh);

protected:
	/// 트랜스폼관련
	/// position / velocity / acceleration 필요
	DirectX::SimpleMath::Vector3 m_Pos;			// world Position
	DirectX::SimpleMath::Vector3 m_StartPos;	// 초기위치 저장
	Velocity m_Velocity;
	DirectX::SimpleMath::Vector3 m_Acceleration;

	// Matrix
	Matrix m_TransTM;		// T
	float m_Angle;			// angle

	COLLISION_OBJECT_TYPE m_CollisionType;

	/// DX 엔진 메쉬 오브젝트
	IDXObjectBase* m_pMesh;	// 메쉬

private:

};