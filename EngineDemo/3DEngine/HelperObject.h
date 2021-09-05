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

	virtual void LoadGeometry(DXGeometryBuffer* pGB); // 기하구조로부터 버텍스/인덱스버퍼를 만든다.
private:
	UINT m_IndexCount;

private:




};

