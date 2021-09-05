#include "pch.h"
#include "IDXObjectBase.h"
#include "Grid.h"

Grid::Grid(IDXObjectBase* pGrid)
	: m_pGrid(pGrid)
{
}

Grid::~Grid()
{
	Release();
}

void Grid::Initialize()
{
	m_pGrid->Initialize();
}

void Grid::Release()
{
	SAFE_DELETE(m_pGrid);
}

void Grid::Update(Camera* pCamera, float dTime)
{
	m_pGrid->Update(pCamera, dTime);
}

void Grid::Render()
{
	m_pGrid->Render();
}
