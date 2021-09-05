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
	void BuildGeometryBuffers(); // ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	void BuildFX();	// ����Ʈ�� �����. (���̴��� ����ϱ� ���ؼ�)
	void BuildVertexLayout(); // ���ؽ����̾ƿ��� �����.


	// ���� �־�� �ϴ�..
private:


};

