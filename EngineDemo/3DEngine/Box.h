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
	virtual void Initialize();	// �ʱ�ȭ
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;		// �׸���. ������ BeginRender()�� EndRender()���̿� ȣ�� �� ��

private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	void BuildFX();						// ����Ʈ�� �����. (���̴��� ����ϱ� ���ؼ�)
	void BuildVertexLayout();			// ���ؽ����̾ƿ��� �����.

private:

	
};

