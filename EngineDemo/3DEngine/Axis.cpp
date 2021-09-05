
#include "Camera.h"

#include "Axis.h"


Axis::Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: IDXObjectBase(pDevice, pDeviceContext, pRS)	
{

}

Axis::~Axis()
{
	ReleaseCOM(m_pFX);
	ReleaseCOM(m_pInputLayout);

	ReleaseCOM(m_pVB);
	ReleaseCOM(m_pIB);
}


void Axis::Initialize()
{
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
}

void Axis::Update(Camera* pCamera, float dTime)
{
	m_World = XMMatrixIdentity();
	m_View = pCamera->View();
	m_Proj = pCamera->Proj();
}

void Axis::Render()
{
	// �Է� ��ġ ��ü ����
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM���� ����
	// Set constants
	Matrix worldViewProj = m_World * m_View * m_Proj;
	m_pFXWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// ����������Ʈ
	m_pDeviceContext->RSSetState(m_pRenderState);

	// ��ũ����...
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc(&techDesc);

	// �����н���...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);

		// 6���� �ε����� ���� �׸���.
		m_pDeviceContext->DrawIndexed(6, 0, 0);
	}
}

void Axis::BuildGeometryBuffers()
{
	// ���� ���۸� �����Ѵ�. 
	// �� �࿡ �µ��� 6���� ������ �������.
	Vertex vertices[] =
	{
		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Red)  },	// x�� (����)
		{ Vector3(10.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Red)  },

		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Green)},	// y�� (�ʷ�)
		{ Vector3(0.0f, 10.0f, 0.0f), Vector4((const float*)&Colors::Green)},

		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
		{ Vector3(0.0f, 0.0f, 10.0f), Vector4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &m_pVB));


	// �ε��� ���۸� �����Ѵ�.
	// ���� 3���� ���� ��Ÿ������ �ߴ�.

	UINT indices[] = {
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
		4, 5,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}

void Axis::BuildFX()
{
	std::ifstream fin("../../Library/FX/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_pDevice, &m_pFX));

	m_pTech = m_pFX->GetTechniqueByName("ColorTech");
	m_pFXWorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Axis::BuildVertexLayout()
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
		passDesc.IAInputSignatureSize, &m_pInputLayout));
}
