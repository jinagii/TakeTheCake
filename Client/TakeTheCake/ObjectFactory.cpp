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
    /// Factory ���� �� �ʱ�ȭ
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
    /// ���޹��� �Ķ���͵�� �޽��� �����ؼ� ������ ������Ʈ�� �����ϰ� �����Ѵ�.
    /// </summary>
    /// <param name="type">������Ʈ Ÿ��</param>
    /// <param name="scale">�ʱ� ������ ��</param>
    /// <param name="angleY">�ʱ� yaw ��</param>
    /// <param name="angleX">�ʱ� pitch ��</param>
    /// <param name="angleZ">�ʱ� roll ��</param>
    /// <param name="pos">�ʱ� positon ���� ��</param>
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

    // �ʱ�ȭ
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