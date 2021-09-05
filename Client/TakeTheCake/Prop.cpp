#include "pch.h"
#include "IDXObjectBase.h"
#include "Scene03_InGame.h"
#include "Prop.h"

Prop::Prop(IDXObjectBase* pMesh)
	: m_pMesh(nullptr)
{
}

Prop::~Prop()
{
	Release();
}

void Prop::Initialize()
{
	IObjectBase::Initialize();
}

void Prop::Release()
{
	//SAFE_DELETE(m_pMesh);
}

void Prop::Update(Camera* pCamera, float dTime)
{
	/////////////////////////////////////////////
	// Update Mesh
	float offsetX = m_Pos.x;
	float offsetY = m_Pos.y;
	float offsetZ = m_Pos.z;

	Matrix matrix;
	matrix = XMMatrixTranslation(offsetX, offsetY, offsetZ);

	m_pMesh->SetTransTM(matrix);

	m_pMesh->Update(pCamera, dTime);
}

void Prop::Render()
{
	if (Scene03_InGame::m_bIsMapRender == true)
	{
		m_pMesh->Render();
	}
	
}

IDXObjectBase* Prop::GetMesh() const
{
	return m_pMesh;
}

void Prop::SetMesh(IDXObjectBase* pMesh)
{
	// 이미 매쉬가 있으면
	if (m_pMesh)
	{
		// 기존 메쉬를 지우고
		SAFE_DELETE(m_pMesh);
	}

	// 새 매쉬로 변경
	m_pMesh = pMesh;
}
