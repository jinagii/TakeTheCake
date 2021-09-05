#pragma once


#include "d3dx11Effect.h"	
#include "DXDefine.h"
#include "IDXObjectBase.h"
#include "GeometryHelper.h"

class HelperObject : public IDXObjectBase
{
public:
	HelperObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~HelperObject();

public:
	virtual void Initialize() override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;

	virtual void LoadGeometry(DXGeometryBuffer* pGB); // ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
private:
	UINT m_IndexCount;

private:




};

