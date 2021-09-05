#pragma once

#include "IDXObjectBase.h"
#include "GeometryHelper.h"


/// Light ������ �� skull
class Skull : public IDXObjectBase
{
public:
	Skull(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~Skull();

public:
	//void Initialize();	// �ʱ�ȭ
	//virtual void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj) override;
	virtual void Update(Camera* pCamera, float dTime) override;
	virtual void Render() override;		// �׸���. ������ BeginRender()�� EndRender()���̿� ȣ�� �� ��

	virtual void LoadGeometry(DXGeometryBuffer* pGB);


private:

	Matrix m_SkullWorld;

	// Light setting
	Material m_SkullMaterial;
	Vector3 m_EyePosW;

	UINT m_SkullIndexCount;
};

