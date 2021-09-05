#include "GeometryHelper.h"
#include "Effects.h"

GeometryHelper::GeometryHelper(ID3D11Device* pDevice)
	:m_pDevice(pDevice)
{

}

GeometryHelper::~GeometryHelper()
{
}

DXGeometryBuffer* GeometryHelper::BuildGeometryBuffer_Grid()
{
	DXGeometryBuffer* _newGridBuffer = new DXGeometryBuffer();

	// 정점 버퍼를 생성한다. 
	PosColor vertices[121];
	for (int i = 0; i < 121; i++)
	{
		vertices[i].Pos = Vector3((float)(i % 11) - 5.0f, 0.0f, (float)(i / 11) - 5.0f);
		vertices[i].Color = Vector4((const float*)&Colors::DarkGray);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosColor) * 121;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &_newGridBuffer->pVB));

	// 인덱스 버퍼를 생성한다.
	UINT indices[44];
	for (int i = 0; i < 11; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 110;
	}

	for (int i = 0; i < 11; i++)
	{
		indices[22 + (i * 2)] = i * 11;
		indices[22 + (i * 2) + 1] = i * 11 + 10;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 44;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &_newGridBuffer->pIB));
	_newGridBuffer->IndexCnt = 44;

	return _newGridBuffer;
}

DXGeometryBuffer* GeometryHelper::BuildGeometryBuffer_Axis()
{
	DXGeometryBuffer* _newAxisBuffer = new DXGeometryBuffer();

	PosColor vertices[] =
	{
		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Red)  },	// x축 (빨강)
		{ Vector3(10.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Red)  },

		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Green)},	// y축 (초록)
		{ Vector3(0.0f, 10.0f, 0.0f), Vector4((const float*)&Colors::Green)},

		{ Vector3(0.0f, 0.0f, 0.0f), Vector4((const float*)&Colors::Blue)	},	// z축 (파랑)
		{ Vector3(0.0f, 0.0f, 10.0f), Vector4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosColor) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &_newAxisBuffer->pVB));

	// 인덱스 버퍼를 생성한다.
	UINT indices[] = {
		// x축
		0, 1,
		// y축
		2, 3,
		// z축
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
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &_newAxisBuffer->pIB));
	_newAxisBuffer->IndexCnt = 6;

	return _newAxisBuffer;
}

DXGeometryBuffer* GeometryHelper::BuildGeometryBuffer_Skull()
{
	DXGeometryBuffer* _newSkull = new DXGeometryBuffer();

	// 텍스트 파일에서 버텍스 정보를 읽음
	std::ifstream fin("../../Library/Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"skull.txt not found.", 0, 0);
		return nullptr;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount; // 'VertexCount' 무시 후 카운트
	fin >> ignore >> tcount; // 'TriangleCount' 무시 후 카운트
	fin >> ignore >> ignore >> ignore >> ignore; // 'VertexList' '(pos,' 'normal)' '{' 무시

	std::vector<PosNor> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore; // '}' 무시
	fin >> ignore; // 'TriangleList' 무시
	fin >> ignore; // '{' 무시

	_newSkull->IndexCnt = 3 * tcount;
	
	std::vector<UINT> indices(_newSkull->IndexCnt);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosNor) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &_newSkull->pVB));

	// Pack the indices of all the meshes into one index buffer.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * _newSkull->IndexCnt;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &_newSkull->pIB));

	return _newSkull;
}

DXGeometryBuffer* GeometryHelper::BuildGeometryBuffer_Image()
{
   DXGeometryBuffer* _newImageBuff = new DXGeometryBuffer();

	_newImageBuff->VertexCnt = 6; 
	_newImageBuff->IndexCnt = 6;

   // 정점 배열을 만듭니다.
   PosTex* _vertices = new PosTex[_newImageBuff->VertexCnt];

   // 정점 배열을 0으로 초기화합니다.
   memset(_vertices, 0, (sizeof(PosTex) * _newImageBuff->VertexCnt));

   // 인덱스 배열을 만듭니다.
	UINT* _indices = new UINT[_newImageBuff->IndexCnt];


	_indices[0] = 2;
	_indices[1] = 1;
	_indices[2] = 0;
	_indices[3] = 5;
	_indices[4] = 4;
	_indices[5] = 3;

   // 데이터로 인덱스 배열을 로드합니다.
 /*  for (UINT i = 0; i < _newImageBuff->IndexCnt; i++)
   {
      _indices[i] = i;
   }*/



   // 정적 정점 버퍼의 구조체를 설정합니다.
   D3D11_BUFFER_DESC vertexBufferDesc;
   vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
   vertexBufferDesc.ByteWidth = sizeof(PosTex) * _newImageBuff->VertexCnt;
   vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   vertexBufferDesc.MiscFlags = 0;
   vertexBufferDesc.StructureByteStride = 0;

   // subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
   D3D11_SUBRESOURCE_DATA vertexData;
   vertexData.pSysMem = _vertices;
   vertexData.SysMemPitch = 0;
   vertexData.SysMemSlicePitch = 0;

   // 이제 정점 버퍼를 만듭니다.
   HR(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &_newImageBuff->pVB));

   // 정적 인덱스 버퍼의 구조체를 설정합니다.
   D3D11_BUFFER_DESC indexBufferDesc;
   indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
   indexBufferDesc.ByteWidth = sizeof(UINT) * _newImageBuff->IndexCnt;
   indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   indexBufferDesc.CPUAccessFlags = 0;
   indexBufferDesc.MiscFlags = 0;
   indexBufferDesc.StructureByteStride = 0;

   // 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
   D3D11_SUBRESOURCE_DATA indexData;
   indexData.pSysMem = _indices;
   indexData.SysMemPitch = 0;
   indexData.SysMemSlicePitch = 0;

   // 인덱스 버퍼를 생성합니다.
   HR(m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, &_newImageBuff->pIB));

   // 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
   delete[] _vertices;
   _vertices = nullptr;

   delete[] _indices;
   _indices = nullptr;
   
   return _newImageBuff;
}
