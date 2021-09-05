#pragma once
#include <DirectXColors.h>	
#include "d3dx11Effect.h"	
#include "DXDefine.h"
#include "IDXObjectBase.h"


class Grid : public IDXObjectBase
{
public:
	Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	virtual ~Grid();

	struct Vertex
	{
		Vector3 Pos;
		Vector4 Color;
	};

public:
	void Initialize();	
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override; 

private:
	void BuildGeometryBuffers(); // 기하구조로부터 버텍스/인덱스버퍼를 만든다.
	void BuildFX();	// 이펙트를 만든다. (쉐이더를 사용하기 위해서)
	void BuildVertexLayout(); // 버텍스레이아웃을 만든다.


	// 뭐뭐 있어야 하니..
private:


};

