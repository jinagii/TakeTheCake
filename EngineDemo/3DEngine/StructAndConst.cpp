#include "DXDefine.h"
#include "d3dx11effect.h"
#include "Effect.h"

#include "StructAndConst.h"

/// InputLayoutDescription
const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Pos[1] =
{
   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosTex[2] =
{
   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	      0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNor[2] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNorTex[3] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	   0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNorTexTan[4] =
{
   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNorTexSkin[5] =
{
   {"POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"NORMAL",	    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"WEIGHTS",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNorTexTanSkin[6] =
{
   {"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
   {"BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

/// InputLayout
ID3D11InputLayout* InputLayout::Pos = 0;
ID3D11InputLayout* InputLayout::PosTex = 0;
ID3D11InputLayout* InputLayout::PosColor = 0;
ID3D11InputLayout* InputLayout::PosNor = 0;
ID3D11InputLayout* InputLayout::PosNorTex = 0;
ID3D11InputLayout* InputLayout::PosNorTexTan = 0;
ID3D11InputLayout* InputLayout::PosNorTexSkin = 0;
ID3D11InputLayout* InputLayout::PosNorTexTanSkin = 0;

void InputLayout::InitializeAll(ID3D11Device* pDevice)
{
	D3DX11_PASS_DESC passDesc;

	// Pos
   StaticEffect::SkyFX->SkyTech->GetPassByIndex(0)->GetDesc(&passDesc);
   HR(pDevice->CreateInputLayout(InputLayoutDesc::Pos, 1, passDesc.pIAInputSignature,
      passDesc.IAInputSignatureSize, &Pos));

   // PosTex
   StaticEffect::BitmapFX->LightTech->GetPassByIndex(0)->GetDesc(&passDesc);
   HR(pDevice->CreateInputLayout(InputLayoutDesc::PosTex, 2, passDesc.pIAInputSignature,
      passDesc.IAInputSignatureSize, &PosTex));

	// PosColor
	StaticEffect::ColorFX->LightTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(pDevice->CreateInputLayout(InputLayoutDesc::PosColor, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosColor));

	// PosNormal
   StaticEffect::BasicFX->PosNormal->GetPassByIndex(0)->GetDesc(&passDesc);
   HR(pDevice->CreateInputLayout(InputLayoutDesc::PosNor, 2, passDesc.pIAInputSignature,
   	passDesc.IAInputSignatureSize, &PosNor));

	// PosNormalTex
	StaticEffect::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(pDevice->CreateInputLayout(InputLayoutDesc::PosNorTex, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosNorTex));

	// PosNormalTexTan
   //StaticEffect::NormalMapFX->Light2Tech->GetPassByIndex(0)->GetDesc(&passDesc);
   //HR(pDevice->CreateInputLayout(InputLayoutDesc::PosNorTexTan, 4, passDesc.pIAInputSignature,
   //   passDesc.IAInputSignatureSize, &PosNorTexTan));

   // PosNormalTexSkin
   //StaticEffect::SkinnedMeshFX->Light1SkinnedTech->GetPassByIndex(0)->GetDesc(&passDesc);
   //HR(pDevice->CreateInputLayout(InputLayoutDesc::PosNorTexSkin, 5, passDesc.pIAInputSignature,
   //   passDesc.IAInputSignatureSize, &PosNorTexSkin));

   // PosNormalTexTanSkin
   //StaticEffect::SkinnedMeshFX->Light1SkinnedTech->GetPassByIndex(0)->GetDesc(&passDesc);
   //HR(pDevice->CreateInputLayout(InputLayoutDesc::PosNorTexTanSkin, 6, passDesc.pIAInputSignature,
   //   passDesc.IAInputSignatureSize, &PosNorTexTanSkin));
}

void InputLayout::FinalizeAll()
{
	ReleaseCOM(Pos);
	ReleaseCOM(PosTex);
	ReleaseCOM(PosNorTex);
	ReleaseCOM(PosNorTexTan);
	ReleaseCOM(PosNor);
	ReleaseCOM(PosColor);
	ReleaseCOM(PosNorTexTanSkin);
	ReleaseCOM(PosNorTexSkin);
}
