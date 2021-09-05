#pragma once

/// structs ///////////////////////////////////////////////////
struct Pos
{
   XMFLOAT3 Pos;
};

struct PosTex
{
	Vector3 Pos;
	Vector2 Tex;
};

struct PosColor 
{
	Vector3 Pos;
	Vector4 Color;
};

struct PosNor 
{
	Vector3 Pos;
	Vector3 Normal;
};

struct PosNorTex
{
	Vector3 Pos;
	Vector3 Normal;
	Vector2 Tex;
};

struct PosNorTexTan
{
	Vector3 Pos;
	Vector3 Normal;
	Vector2 Tex;
	Vector3 TangentU;
};

////////////////////////////////////////////////////////////////////////// skinning
struct PosNorTexSkin
{
   Vector3 Pos;
	Vector3 Normal;
	Vector2 Tex;
	Vector3 Weights;
   BYTE BoneIndices[4];
};

struct PosNorTexTanSkin 
{
	Vector3 Pos;
	Vector3 Normal;
	Vector2 Tex;
	Vector4 TangentU;
	Vector3 Weights;
   BYTE BoneIndices[4];
};

/// InputLayout ////////////////////////////////////////////////
class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC Pos[1];
	static const D3D11_INPUT_ELEMENT_DESC PosTex[2];
	static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNor[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNorTex[3];
	static const D3D11_INPUT_ELEMENT_DESC PosNorTexTan[4];

	static const D3D11_INPUT_ELEMENT_DESC PosNorTexSkin[5];
	static const D3D11_INPUT_ELEMENT_DESC PosNorTexTanSkin[6];
};

class InputLayout
{
public:
	static void InitializeAll(ID3D11Device* pDevice);
	static void FinalizeAll();

	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* PosTex;
	static ID3D11InputLayout* PosColor;
	static ID3D11InputLayout* PosNor;
	static ID3D11InputLayout* PosNorTex;
	static ID3D11InputLayout* PosNorTexTan;

	static ID3D11InputLayout* PosNorTexSkin;
	static ID3D11InputLayout* PosNorTexTanSkin;
};

