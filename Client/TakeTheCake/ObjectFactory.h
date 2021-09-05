#pragma once

/// <summary>
/// ObjectFactory Class
/// Writer : YoKing
/// 
/// 게임 내에서 사용할 오브젝트를 생성하기 위한 팩토리 클래스
/// DX 엔진에서 Mesh 를 생성해오기 위해 DXObjectFactory 정보도 필요
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
	FOLLOWER,	// 원래는 위의 Flock, Boid, Ant와 연동되어야 하나 임시로 만듦 (2021.08.27, LeHide)
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



	/// 오브젝트 생성
	class IObjectBase* CreateObject(
		const OBJECT_TYPE type, 
		const float scale = 1.0f, 
		const float angleY = 0.0f, 
		const float angleX = 0.0f, 
		const float angleZ = 0.0f, 
		const Vector3 pos = DirectX::SimpleMath::Vector3::Zero
	) const;

	/// 메쉬만 만들기
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