#pragma once

#include "DXDefine.h"
#include "d3dx11Effect.h"	

class Effect
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs); // 언제 쓰지?

protected:
	ID3DX11Effect* m_pFX;
};

//////////////////////////////////////////////////////////////////////////

class ColorEffect :Effect
{
public:
	ColorEffect(ID3D11Device* pDevice, const std::wstring& filename);
	~ColorEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* LightTech;
	ID3DX11EffectTechnique* LightTech2;

	ID3DX11EffectMatrixVariable* WorldViewProj;
};

//////////////////////////////////////////////////////////////////////////

class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* pDevice, const std::wstring& filename);
	~BasicEffect();

   void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
   void SetFogColor(const FXMVECTOR v) { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
   void SetFogStart(float f) { FogStart->SetFloat(f); }
   void SetFogRange(float f) { FogRange->SetFloat(f); }
   void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
   void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
   void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
   void SetCubeMap(ID3D11ShaderResourceView* tex) { CubeMap->SetResource(tex); }

	ID3DX11EffectTechnique* PosNormal;

   ID3DX11EffectTechnique* Light1Tech;
   ID3DX11EffectTechnique* Light2Tech;
   ID3DX11EffectTechnique* Light3Tech;

   ID3DX11EffectTechnique* Light0TexTech;
   ID3DX11EffectTechnique* Light1TexTech;
   ID3DX11EffectTechnique* Light2TexTech;
   ID3DX11EffectTechnique* Light3TexTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipTech;

   ID3DX11EffectTechnique* Light1FogTech;
   ID3DX11EffectTechnique* Light2FogTech;
   ID3DX11EffectTechnique* Light3FogTech;

   ID3DX11EffectTechnique* Light0TexFogTech;
   ID3DX11EffectTechnique* Light1TexFogTech;
   ID3DX11EffectTechnique* Light2TexFogTech;
   ID3DX11EffectTechnique* Light3TexFogTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

   ID3DX11EffectTechnique* Light1ReflectTech;
   ID3DX11EffectTechnique* Light2ReflectTech;
   ID3DX11EffectTechnique* Light3ReflectTech;

   ID3DX11EffectTechnique* Light0TexReflectTech;
   ID3DX11EffectTechnique* Light1TexReflectTech;
   ID3DX11EffectTechnique* Light2TexReflectTech;
   ID3DX11EffectTechnique* Light3TexReflectTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

   ID3DX11EffectTechnique* Light1FogReflectTech;
   ID3DX11EffectTechnique* Light2FogReflectTech;
   ID3DX11EffectTechnique* Light3FogReflectTech;

   ID3DX11EffectTechnique* Light0TexFogReflectTech;
   ID3DX11EffectTechnique* Light1TexFogReflectTech;
   ID3DX11EffectTechnique* Light2TexFogReflectTech;
   ID3DX11EffectTechnique* Light3TexFogReflectTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

   ID3DX11EffectMatrixVariable* WorldViewProj;
   ID3DX11EffectMatrixVariable* World;
   ID3DX11EffectMatrixVariable* WorldInvTranspose;
   ID3DX11EffectMatrixVariable* TexTransform;
   ID3DX11EffectVectorVariable* EyePosW;
   ID3DX11EffectVectorVariable* FogColor;
   ID3DX11EffectScalarVariable* FogStart;
   ID3DX11EffectScalarVariable* FogRange;
   ID3DX11EffectVariable* DirLights;
   ID3DX11EffectVariable* Mat;

   ID3DX11EffectShaderResourceVariable* DiffuseMap;
   ID3DX11EffectShaderResourceVariable* CubeMap;
};

//////////////////////////////////////////////////////////////////////////

class NormalMapEffect : public Effect
{
public:
   NormalMapEffect(ID3D11Device* device, const std::wstring& filename);
   ~NormalMapEffect();

   void SetWorldViewProj(Matrix M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorld(Matrix M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorldInvTranspose(Matrix M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetTexTransform(Matrix M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetEyePosW(const Vector3& v) { EyePosW->SetRawValue(&v, 0, sizeof(Vector3)); }
   void SetFogColor(const Vector4 v) { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
   void SetFogStart(float f) { FogStart->SetFloat(f); }
   void SetFogRange(float f) { FogRange->SetFloat(f); }
   void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
   void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
   void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
   void SetCubeMap(ID3D11ShaderResourceView* tex) { CubeMap->SetResource(tex); }
   void SetNormalMap(ID3D11ShaderResourceView* tex) { NormalMap->SetResource(tex); }

   ID3DX11EffectTechnique* Light1Tech;
   ID3DX11EffectTechnique* Light2Tech;
   ID3DX11EffectTechnique* Light3Tech;

   ID3DX11EffectTechnique* Light0TexTech;
   ID3DX11EffectTechnique* Light1TexTech;
   ID3DX11EffectTechnique* Light2TexTech;
   ID3DX11EffectTechnique* Light3TexTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipTech;

   ID3DX11EffectTechnique* Light1FogTech;
   ID3DX11EffectTechnique* Light2FogTech;
   ID3DX11EffectTechnique* Light3FogTech;

   ID3DX11EffectTechnique* Light0TexFogTech;
   ID3DX11EffectTechnique* Light1TexFogTech;
   ID3DX11EffectTechnique* Light2TexFogTech;
   ID3DX11EffectTechnique* Light3TexFogTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

   ID3DX11EffectTechnique* Light1ReflectTech;
   ID3DX11EffectTechnique* Light2ReflectTech;
   ID3DX11EffectTechnique* Light3ReflectTech;

   ID3DX11EffectTechnique* Light0TexReflectTech;
   ID3DX11EffectTechnique* Light1TexReflectTech;
   ID3DX11EffectTechnique* Light2TexReflectTech;
   ID3DX11EffectTechnique* Light3TexReflectTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

   ID3DX11EffectTechnique* Light1FogReflectTech;
   ID3DX11EffectTechnique* Light2FogReflectTech;
   ID3DX11EffectTechnique* Light3FogReflectTech;

   ID3DX11EffectTechnique* Light0TexFogReflectTech;
   ID3DX11EffectTechnique* Light1TexFogReflectTech;
   ID3DX11EffectTechnique* Light2TexFogReflectTech;
   ID3DX11EffectTechnique* Light3TexFogReflectTech;

   ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
   ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

   ID3DX11EffectMatrixVariable* WorldViewProj;
   ID3DX11EffectMatrixVariable* World;
   ID3DX11EffectMatrixVariable* WorldInvTranspose;
   ID3DX11EffectMatrixVariable* TexTransform;
   ID3DX11EffectVectorVariable* EyePosW;
   ID3DX11EffectVectorVariable* FogColor;
   ID3DX11EffectScalarVariable* FogStart;
   ID3DX11EffectScalarVariable* FogRange;
   ID3DX11EffectVariable* DirLights;
   ID3DX11EffectVariable* Mat;

   ID3DX11EffectShaderResourceVariable* DiffuseMap;
   ID3DX11EffectShaderResourceVariable* CubeMap;
   ID3DX11EffectShaderResourceVariable* NormalMap;
};

//////////////////////////////////////////////////////////////////////////

class SkinnedMeshEffect : public Effect
{
public:
   SkinnedMeshEffect(ID3D11Device* device, const std::wstring& filename);
   ~SkinnedMeshEffect();

   void SetWorldViewProj(Matrix M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorld(Matrix M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetWorldInvTranspose(Matrix M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetBoneTransforms(const Matrix* M, int cnt) { BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
   void SetTexTransform(Matrix M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetEyePosW(const Vector3& v) { EyePosW->SetRawValue(&v, 0, sizeof(Vector3)); }
   void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
   void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
   void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }


   ID3DX11EffectTechnique* Light1SkinnedTech;
   ID3DX11EffectTechnique* Light2SkinnedTech;
   ID3DX11EffectTechnique* Light3SkinnedTech;

   ID3DX11EffectTechnique* Light0TexSkinnedTech;
   ID3DX11EffectTechnique* Light1TexSkinnedTech;
   ID3DX11EffectTechnique* Light2TexSkinnedTech;
   ID3DX11EffectTechnique* Light3TexSkinnedTech;


   ID3DX11EffectMatrixVariable* WorldViewProj;
   ID3DX11EffectMatrixVariable* World;
   ID3DX11EffectMatrixVariable* WorldInvTranspose;
   ID3DX11EffectMatrixVariable* BoneTransforms;
   ID3DX11EffectMatrixVariable* TexTransform;
   ID3DX11EffectVectorVariable* EyePosW;
   ID3DX11EffectVariable* DirLights;
   ID3DX11EffectVariable* Mat;

   ID3DX11EffectShaderResourceVariable* DiffuseMap;
};

//////////////////////////////////////////////////////////////////////////

class SkyEffect : public Effect
{
public:
   SkyEffect(ID3D11Device* device, const std::wstring& filename);
   ~SkyEffect();

   void SetWorldViewProj(Matrix M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetCubeMap(ID3D11ShaderResourceView* cubemap) { CubeMap->SetResource(cubemap); }

   ID3DX11EffectTechnique* SkyTech;

   ID3DX11EffectMatrixVariable* WorldViewProj;

   ID3DX11EffectShaderResourceVariable* CubeMap;
};

//////////////////////////////////////////////////////////////////////////

class BlurEffect : public Effect
{
public:
	BlurEffect(ID3D11Device* pDevice, const std::wstring& filename);
	~BlurEffect();

	void SetWeights(const float weights[9]) { Weights->SetFloatArray(weights, 0, 9); }
	void SetInputMap(ID3D11ShaderResourceView* tex) { InputMap->SetResource(tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* tex) { OutputMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectTechnique* HorzBlurTech;
	ID3DX11EffectTechnique* VertBlurTech;

	ID3DX11EffectScalarVariable* Weights;
	ID3DX11EffectShaderResourceVariable* InputMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;
};

//////////////////////////////////////////////////////////////////////////

class BitmapEffect : public Effect
{
public:
   BitmapEffect(ID3D11Device* device, const std::wstring& filename);
   ~BitmapEffect();

   void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
   void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }

   ID3DX11EffectMatrixVariable* WorldViewProj;
   ID3DX11EffectShaderResourceVariable* DiffuseMap;
   ID3DX11EffectTechnique* LightTech;
};

//------------------------------------------------------------------------------
class StaticEffect
{
public:
	static void InitializeAll(ID3D11Device* pDevice);
	static void FinalizeAll();

	static ColorEffect* ColorFX;
	static BasicEffect* BasicFX;
   //static NormalMapEffect* NormalMapFX;
   //static SkinnedMeshEffect* SkinnedMeshFX;

   static SkyEffect* SkyFX;
	static BlurEffect* BlurFX;
   static BitmapEffect* BitmapFX;
};

