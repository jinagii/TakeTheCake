/// <summary>
/// IObjectBase Class
/// Writer : �� �� ȯ
/// 
/// ������Ʈ���� �⺻�� �Ǵ� Ŭ����
/// 
/// �߻�Ŭ������ ����� �Ϻ� Ŭ�������� ����ؼ� �Լ��� �����ϰ� �����.
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

// �����ϴ� ������ �ִ��� ���󰡱� ���Ͽ� Velocity ����ü�� ����
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

	DirectX::SimpleMath::Vector3 direction;	// �ӵ�
	float magnitude;						// ũ��
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

	/// Ȯ�强�� ����... ���߿� �޽��� �����ؾ��� ���� ����ٸ�...
	virtual IDXObjectBase* GetMesh() const;
	virtual void SetMesh(class IDXObjectBase* pMesh);

protected:
	/// Ʈ����������
	/// position / velocity / acceleration �ʿ�
	DirectX::SimpleMath::Vector3 m_Pos;			// world Position
	DirectX::SimpleMath::Vector3 m_StartPos;	// �ʱ���ġ ����
	Velocity m_Velocity;
	DirectX::SimpleMath::Vector3 m_Acceleration;

	// Matrix
	Matrix m_TransTM;		// T
	float m_Angle;			// angle

	COLLISION_OBJECT_TYPE m_CollisionType;

	/// DX ���� �޽� ������Ʈ
	IDXObjectBase* m_pMesh;	// �޽�

private:

};