#include "DXDefine.h"
#include <DirectXColors.h>
#include "d3dx11Effect.h"	// effect, tech
#include "StructAndConst.h"
#include "Effect.h"
#include "DXRenderState.h"
#include "Camera.h"

#include "Skull.h"

Skull::Skull(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: IDXObjectBase(pDevice, pDeviceContext),
	m_EyePosW(0.0f, 0.0f, 0.0f), m_SkullWorld(), m_SkullIndexCount()
{
	m_SkullMaterial.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_SkullMaterial.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_SkullMaterial.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

Skull::~Skull()
{

}

void Skull::Update(Camera* pCamera, float dTime)
{
	m_World = XMMatrixIdentity();
	m_View = pCamera->View();
	m_Proj = pCamera->Proj();

	if (GetAsyncKeyState('0') & 0x8000)	m_RenderingIndex = 1;
	if (GetAsyncKeyState('1') & 0x8000)	m_RenderingIndex = 2;
	if (GetAsyncKeyState('2') & 0x8000)	m_RenderingIndex = 3;

	m_EyePosW = Vector3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);

	static float _rot = 0.0f;
	Vector3 _r = Vector3(0.0f, 1.0f, 0.0f);
	//m_SkullWorld = XMMatrixRotationAxis(XMLoadFloat3(&_r), _rot);
	_rot += dTime;
}

void Skull::Render()
{
	// 입력배치 객체 세팅
	m_pDeviceContext->IASetInputLayout(InputLayout::PosNor);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDeviceContext->RSSetState(DXRenderState::g_SolidFrame);

	// buffer setting
	UINT _stride = sizeof(PosNor);
	UINT _offset = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &_stride, &_offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	// world view projection TM setting
	Matrix _world = m_World;
	Matrix _worldViewProj = _world * m_View * m_Proj;

	// 월드 역행렬
	Matrix _worldInvTranspose = MathHelper::InverseTranspose(_world);

	StaticEffect::BasicFX->SetDirLights(m_TempLight);
	StaticEffect::BasicFX->SetEyePosW(m_EyePosW);

	// technique choice
	m_pTech = StaticEffect::BasicFX->PosNormal;

		// 테크닉
	D3DX11_TECHNIQUE_DESC _techDesc;
	m_pTech->GetDesc(&_techDesc);

	// 랜더패스
	for (UINT p = 0; p < _techDesc.Passes; ++p)
	{
		_world = m_SkullWorld;
		_worldInvTranspose = MathHelper::InverseTranspose(_world);
		_worldViewProj = _world * m_View * m_Proj;
		StaticEffect::BasicFX->SetWorld(_world);
		StaticEffect::BasicFX->SetWorldInvTranspose(_worldInvTranspose);
		StaticEffect::BasicFX->SetWorldViewProj(_worldViewProj);
		StaticEffect::BasicFX->SetMaterial(m_SkullMaterial);

		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawIndexed(m_SkullIndexCount, 0, 0);
	}
}

void Skull::LoadGeometry(DXGeometryBuffer* pGB)
{
	m_pVB = pGB->pVB;
	m_pIB = pGB->pIB;
	m_SkullIndexCount = pGB->IndexCnt;
}
