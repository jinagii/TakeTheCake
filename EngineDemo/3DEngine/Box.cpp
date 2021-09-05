#include "DXRenderState.h"
#include "Camera.h"
#include "StructAndConst.h"
#include "Box.h"

Box::Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: IDXObjectBase(pDevice, pDeviceContext)
{

}

Box::~Box()
{
	/// 릴리즈/삭제는 할당/생성의 반대 순서로
	ReleaseCOM(m_pFX);

	ReleaseCOM(m_pIB);
	ReleaseCOM(m_pVB);
}

void Box::Initialize()
{
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
}

void Box::Update(Camera* pCamera, float dTime)
{
	m_World = XMMatrixIdentity();
	m_View = pCamera->View();
	m_Proj = pCamera->Proj();
}

void Box::Render()
{
	// 입력 배치 객체 셋팅
	m_pDeviceContext->IASetInputLayout(InputLayout::PosColor);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_pDeviceContext->RSSetState(DXRenderState::g_SolidFrame);

	// 버텍스버퍼와 인덱스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	Matrix view = m_View;
	Matrix proj = m_Proj;
	Matrix world = m_World;
	Matrix worldViewProj = world * view * proj;

	m_pFXWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc(&techDesc);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawIndexed(36, 0, 0);
	}
}

void Box::BuildGeometryBuffers()
{
	/// 버텍스 버퍼를 생성한다.
	// 정육면체를 찍는 것이 아름답겠지만, 투영행렬이 없거나 할 때도 제대로 보이도록 
	// 위치를 살짝 조정한 사다리꼴형태로 만들었다.

	Vertex vertices[] =
	{
		{ Vector3(-1.0f, -1.0f, -0.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(-1.0f, +1.0f, 0.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(+1.0f, +1.0f, 0.0f), Vector4((const float*)&Colors::Purple) },	// 우상 증가
		{ Vector3(+1.0f, -1.0f, 0.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector4((const float*)&Colors::Purple) },
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector4((const float*)&Colors::Purple) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &m_pVB));

	/// 인덱스 버퍼를 생성한다.
	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}

/// 용책 예제에서는 컴파일을 해서 쓰지만, 여기에서는 이미 컴파일된 파일을 읽어서 생성한다.
void Box::BuildFX()
{
	std::ifstream fin("../../Library/FX/color.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, m_pDevice, &m_pFX));

	m_pTech = m_pFX->GetTechniqueByName("ColorTech");
	m_pFXWorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Box::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &InputLayout::PosColor));
}
