#pragma once
#include "DXDefine.h"
#include <DirectXColors.h>	
#include "d3dx11Effect.h"	

#include "IDXObjectBase.h"


// 축을 그려주는 부분
class Axis : public IDXObjectBase
{
public:
	Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	virtual ~Axis();

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


	// 박스를 그리기 위해서는 뭐뭐 있어야 하니..
private:


};

