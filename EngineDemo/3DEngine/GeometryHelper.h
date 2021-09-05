#pragma once
#include "AlignedAllocationPolicy.h"
#include "DXDefine.h"
#include "StructAndConst.h"

/// GeometryBuffer를 생성해주는 Helper 구초체와 클래스
/// 2021.8.1 JinHak

struct DXGeometryBuffer
{
	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;
	UINT IndexCnt;
	UINT VertexCnt;
	//ID3D11Device* pDevice;
};

class GeometryHelper : public AlignedAllocationPolicy<16>
{
public:
	GeometryHelper(ID3D11Device* pDevice);
	~GeometryHelper();

	// LineList
	DXGeometryBuffer* BuildGeometryBuffer_Grid();
	DXGeometryBuffer* BuildGeometryBuffer_Axis();


	// TriangleList
	DXGeometryBuffer* BuildGeometryBuffer_Skull();
	DXGeometryBuffer* BuildGeometryBuffer_Image();

private:

	ID3D11Device* m_pDevice;
};

