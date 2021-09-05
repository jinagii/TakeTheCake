#include "pch.h"
#include "IDXObjectBase.h"
#include "Axis.h"

Axis::Axis(IDXObjectBase* pAxis)
	: m_pAxis(pAxis)
{
	
}

Axis::~Axis()
{
	Release();
}

void Axis::Initialize()
{
}

void Axis::Release()
{
	SAFE_DELETE(m_pAxis);
}

void Axis::Update(Camera* pCamera, float dTime)
{
	m_pAxis->Update(pCamera, dTime);
}

void Axis::Render()
{
	m_pAxis->Render();
}
