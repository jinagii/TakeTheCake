#include "HelperObject.h"
#include "Effect.h"
#include "Camera.h"
#include "DXRenderState.h"

HelperObject::HelperObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: IDXObjectBase(pDevice, pDeviceContext)
{

}

HelperObject::~HelperObject()
{
	ReleaseCOM(m_pFX);

	ReleaseCOM(m_pVB);
	ReleaseCOM(m_pIB);
}


void HelperObject::Initialize()
{

}

void HelperObject::Update(Camera* pCamera, float dTime)
{
	m_World = XMMatrixIdentity();
	m_View = pCamera->View();
	m_Proj = pCamera->Proj();
}

void HelperObject::Render()
{
	// 입력 배치 객체 셋팅
	m_pDeviceContext->IASetInputLayout(InputLayout::PosColor);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(PosColor);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	Matrix worldViewProj = m_World * m_View * m_Proj;
	StaticEffect::ColorFX->SetWorldViewProj(worldViewProj);

	// 렌더스테이트
	m_pDeviceContext->RSSetState(DXRenderState::g_WireFrame);

	m_pTech = StaticEffect::ColorFX->LightTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc(&techDesc);

	// 랜더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);

		// 20개의 인덱스로 그리드를 그린다.
		m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}
}

void HelperObject::LoadGeometry(DXGeometryBuffer* pGB)
{
	m_pIB = pGB->pIB;
	m_pVB = pGB->pVB;

	m_IndexCount = pGB->IndexCnt;
}
