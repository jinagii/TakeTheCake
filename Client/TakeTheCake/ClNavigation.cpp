#include "pch.h"

#include "Navigation.h"
#include "ClNavigation.h"

ClNavigation::ClNavigation(IDXObjectBase* pNavigation)
	: m_pDXNavigation(nullptr)
{
	m_pDXNavigation = dynamic_cast<Navigation*>(pNavigation);
}

ClNavigation::~ClNavigation()
{
	Release();
}

void ClNavigation::Initialize()
{
	IObjectBase::Initialize();
}

void ClNavigation::Release()
{
	SAFE_DELETE(m_pDXNavigation);
}

void ClNavigation::Update(Camera* pCamera, float dTime)
{
	m_pDXNavigation->Update(pCamera, dTime);
}

void ClNavigation::Render()
{
	m_pDXNavigation->Render();
}
