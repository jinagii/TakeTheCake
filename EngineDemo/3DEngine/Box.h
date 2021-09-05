#pragma once
#include "DXDefine.h"
#include <DirectXColors.h>
#include "d3dx11Effect.h"	

#include "IDXObjectBase.h"

class Box : public IDXObjectBase
{
public:
	Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Box();

	struct Vertex
	{
		Vector3 Pos;
		Vector4 Color;
	};

public:
	virtual void Initialize();	// 초기화
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;		// 그린다. 엔진의 BeginRender()와 EndRender()사이에 호출 될 것

private:
	void BuildGeometryBuffers();		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.
	void BuildFX();						// 이펙트를 만든다. (쉐이더를 사용하기 위해서)
	void BuildVertexLayout();			// 버텍스레이아웃을 만든다.

private:

	
};

