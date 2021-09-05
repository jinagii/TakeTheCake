#include "pch.h"
#include "IDXObjectBase.h"
#include "Texture2D.h"

Texture2D::Texture2D(IDXObjectBase* pTexture2D)
	: m_pDxObject(pTexture2D)
{
}

Texture2D::~Texture2D()
{
	Release();
}

void Texture2D::Initialize()
{
	
}

void Texture2D::Release()
{
	SAFE_DELETE(m_pDxObject);
}

void Texture2D::Update(Camera* pCamera, float dTime)
{
	m_pDxObject->Update(pCamera, dTime);
}

void Texture2D::Render()
{
	m_pDxObject->Render();
}
