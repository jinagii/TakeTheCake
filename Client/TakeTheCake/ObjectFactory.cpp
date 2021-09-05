#include "pch.h"
#include "DXObjectFactory.h"
#include "Axis.h"
#include "Grid.h"
#include "DXTKFont.h"
#include "Player.h"
#include "Flock.h"
#include "AntBase.h"
#include "CupCake.h"
#include "Fire.h"
#include "Knife.h"
#include "Trap.h"
#include "Follower.h"

#include "Prop.h"

#include "ClNavigation.h"

// UI
#include "Texture2D.h"

#include "ObjectFactory.h"

ObjectFactory::ObjectFactory()
    : m_DXObjectFactory(nullptr)
{
    /// Factory 생성 및 초기화
    m_DXObjectFactory = new DXObjectFactory(DXDevice::g_Device, DXDevice::g_DeviceContext);
    m_DXObjectFactory->Initialize();

}

ObjectFactory::~ObjectFactory()
{
    Release();
}

void ObjectFactory::Release()
{
    SAFE_DELETE(m_DXObjectFactory);
}

IObjectBase* ObjectFactory::CreateObject(const OBJECT_TYPE type, const float scale, const float angleY, const float angleX, const float angleZ, const Vector3 pos) const
{
    /// <summary>
    /// 전달받은 파라미터들로 메쉬를 생성해서 각각의 오브젝트를 생성하고 리턴한다.
    /// </summary>
    /// <param name="type">오브젝트 타입</param>
    /// <param name="scale">초기 스케일 값</param>
    /// <param name="angleY">초기 yaw 값</param>
    /// <param name="angleX">초기 pitch 값</param>
    /// <param name="angleZ">초기 roll 값</param>
    /// <param name="pos">초기 positon 벡터 값</param>
    /// <returns></returns>

    IObjectBase* object = nullptr;

    switch (type)
    {
    case OBJECT_TYPE::AXIS:
        object = new Axis(m_DXObjectFactory->CreateAxis());
        break;

    case OBJECT_TYPE::GRID:
        object = new Grid(m_DXObjectFactory->CreateGrid());
        break;

    case OBJECT_TYPE::CAMERA:
        break;

    case OBJECT_TYPE::Navigation:
        object = new ClNavigation(m_DXObjectFactory->CreateNavigationMesh());
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::FLOCK:
        object = new Flock();
        break;

    case OBJECT_TYPE::PLAYER:
        object = new Player(CreateMesh("ant", scale, angleY, angleX, angleZ, pos));
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::BOID:
        break;

    case OBJECT_TYPE::ANT:
        object = new AntBase(CreateMesh("ant", scale, angleY, angleX, angleZ, pos));
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::PROP:
        object = new Prop();
        break;

    case OBJECT_TYPE::CUPCAKE:
        object = new CupCake(CreateMesh(GIMMICK_CUPCAKE, scale, angleY, angleX, angleZ, pos));
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::FIRE_A:
        object = new Fire(
            CreateMesh(PROP_OVEN_FIRE_A_OFF, scale, angleY, angleX, angleZ, Vector3::Zero),
            CreateMesh(GIMMICK_OVEN_FIRE_A_ON, scale, angleY, angleX, angleZ, pos)
        );
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::FIRE_B:
        object = new Fire(
            CreateMesh(PROP_OVEN_FIRE_B_OFF, scale, angleY, angleX, angleZ, Vector3::Zero),
            CreateMesh(GIMMICK_OVEN_FIRE_B_ON, scale, angleY, angleX, angleZ, pos)
        );
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::FIRE_C:
        object = new Fire(
            CreateMesh(PROP_OVEN_FIRE_C_OFF, scale, angleY, angleX, angleZ, Vector3::Zero),
            CreateMesh(GIMMICK_OVEN_FIRE_C_ON, scale, angleY, angleX, angleZ, pos)
        );
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::FIRE_D:
        object = new Fire(
            CreateMesh(PROP_OVEN_FIRE_D_OFF, scale, angleY, angleX, angleZ, Vector3::Zero),
            CreateMesh(GIMMICK_OVEN_FIRE_D_ON, scale, angleY, angleX, angleZ, pos)
        );
        object->SetStartPos(pos);
        break;


    case OBJECT_TYPE::KNIFE:
        object = new Knife(CreateMesh(GIMMICK_KNIFE, scale, angleY, angleX, angleZ, pos));
        object->SetStartPos(pos);
        break;

    case OBJECT_TYPE::TRAP:
        object = new Trap(CreateMesh(GIMMICK_TRAP, scale, angleY, angleX, angleZ, pos));
        object->SetStartPos(pos);
        break;

	case OBJECT_TYPE::FOLLOWER:
		object = new Follower(CreateMesh("ant", scale, angleY, angleX, angleZ, pos));
		object->SetStartPos(pos);
		break;

    default:
        break;
    }

    // 초기화
    object->Initialize();

    return object;
}


IDXObjectBase* ObjectFactory::CreateMesh(const std::string type, const float scale, const float angleY, const float angleX, const float angleZ, const Vector3 pos) const
{
    IDXObjectBase* meshObject = nullptr;

    meshObject = m_DXObjectFactory->CreateObjectFromASEParser(type, scale, angleY, angleX, angleZ, pos);
      
    return meshObject;
}

Texture2D* ObjectFactory::CreateImageUI(const std::string name, const int posX, const int posY, const int width, const int height)
{
    Texture2D* imageUI;

    imageUI = new Texture2D( m_DXObjectFactory->CreateImageObject(name, posX, posY, width, height));
   
    imageUI->Initialize();

    return imageUI;
}