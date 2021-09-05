#include "Camera.h"
#include "GeometryHelper.h"
#include "DebugCircle.h"

#include "IDXObjectBase.h"

IDXObjectBase::IDXObjectBase(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
   : m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
   , m_pVB(nullptr)
   , m_pIB(nullptr)
   , m_pFX(nullptr)
   , m_pTech(nullptr)
   , m_pFXWorldViewProj(nullptr)
   , m_World(), m_View(), m_Proj()
   , m_ForwardVecOffset(0.0f, 0.0f, -1.0f)
   , m_Angle(0.0f)
   , m_TempLight()
   , m_RenderingIndex(1)
   , m_pDebugCircle()
{
   //m_TempLight[0].Ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
   //m_TempLight[0].Diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
   //m_TempLight[0].Specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
   //m_TempLight[0].Direction = Vector3(0.57735f, -0.57735f, 0.57735f);
   //
   //m_TempLight[1].Ambient = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
   //m_TempLight[1].Diffuse = Vector4(0.20f, 0.20f, 0.20f, 1.0f);
   //m_TempLight[1].Specular = Vector4(0.25f, 0.25f, 0.25f, 1.0f);
   //m_TempLight[1].Direction = Vector3(-0.57735f, -0.57735f, 0.57735f);
   //
   //m_TempLight[2].Ambient = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
   //m_TempLight[2].Diffuse = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
   //m_TempLight[2].Specular = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
   //m_TempLight[2].Direction = Vector3(0.0f, -0.707f, -0.707f);

}

IDXObjectBase::~IDXObjectBase()
{
   ReleaseCOM(m_pVB);
   ReleaseCOM(m_pIB);

}

void IDXObjectBase::MoveForward(float dTime, float val)
{
   m_TransTM._41 += m_ForwardVecOffset.x * dTime * val;
   m_TransTM._42 += m_ForwardVecOffset.y * dTime * val;
   m_TransTM._43 += m_ForwardVecOffset.z * dTime * val;
}

void IDXObjectBase::RotateObject(float dTime, float val)
{
   m_Angle += dTime * val;
}
