#pragma once

#include "IDXObjectBase.h"
#include "GeometryHelper.h"


/// Light 예제가 들어간 skull
class Skull : public IDXObjectBase
{
public:
	Skull(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Skull();

public:
	//void Initialize();	// 초기화
	//virtual void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj) override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;		// 그린다. 엔진의 BeginRender()와 EndRender()사이에 호출 될 것

	virtual void LoadGeometry(DXGeometryBuffer* pGB);


private:

	Matrix m_SkullWorld;

	// Light setting
	Material m_SkullMaterial;
	Vector3 m_EyePosW;

	UINT m_SkullIndexCount;
};

