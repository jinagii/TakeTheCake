#pragma once

/// <summary>
/// ObjectFactory Class
/// Writer : YoKing
/// 
/// ���� ������ ����� ������Ʈ�� �����ϱ� ���� ���丮 Ŭ����
/// DX �������� Mesh �� �����ؿ��� ���� DXObjectFactory ������ �ʿ�
/// 
/// First Create	  : 2021.08.19
/// Last Modification : 2021.08.23
/// </summary>

#include "SingletonBase.h"

enum class OBJECT_TYPE
{
	AXIS,
	GRID,
	CAMERA,
	Navigation,
	PLAYER,
	FLOCK,
	BOID,
	ANT,
	PROP,
	CUPCAKE,
	FIRE_A,
	FIRE_B,
	FIRE_C,
	FIRE_D,
	KNIFE,
	TRAP,
	TRAP_ANT,
	FOLLOWER,	// ������ ���� Flock, Boid, Ant�� �����Ǿ�� �ϳ� �ӽ÷� ���� (2021.08.27, LeHide)
};

enum class MESH_TYPE
{
	ANT,
	FIRE,
};

enum class UI_TYPE
{
	UI_1,
};

class ObjectFactory : public SingletonBase<ObjectFactory>
{
public:
	ObjectFactory();
	~ObjectFactory();

	virtual void Release() override;



	/// ������Ʈ ����
	class IObjectBase* CreateObject(
		const OBJECT_TYPE type, 
		const float scale = 1.0f, 
		const float angleY = 0.0f, 
		const float angleX = 0.0f, 
		const float angleZ = 0.0f, 
		const Vector3 pos = DirectX::SimpleMath::Vector3::Zero
	) const;

	/// �޽��� �����
	class IDXObjectBase* CreateMesh(
		const std::string type,
		const float scale = 1.0f,
		const float angleY = 0.0f,
		const float angleX = 0.0f,
		const float angleZ = 0.0f,
		const Vector3 pos = DirectX::SimpleMath::Vector3::Zero
	) const;

	class Texture2D* CreateImageUI
	(
		const std::string name,
		const int posX,
		const int posY,
		const int width,
		const int height
	);
protected:

private:
	class DXObjectFactory* m_DXObjectFactory;

};