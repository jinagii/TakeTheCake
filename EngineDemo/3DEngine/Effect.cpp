
#include "Effect.h"

Effect::Effect(ID3D11Device* pDevice, const std::wstring& filename)
	: m_pFX(nullptr)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, pDevice, &m_pFX));
}

Effect::~Effect()
{
	ReleaseCOM(m_pFX);
}

////////////////////////////////////////////////////////////////////////////////////////

ColorEffect::ColorEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	LightTech = m_pFX->GetTechniqueByName("ColorTech");
	WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

ColorEffect::~ColorEffect()
{
}

///////////////////////////////////////////////////////////////////////////////////////

BasicEffect::BasicEffect(ID3D11Device* pDevice, const std::wstring& filename)
	: Effect(pDevice, filename)
{
	PosNormal = m_pFX->GetTechniqueByName("PosNormal");

   Light1Tech =m_pFX->GetTechniqueByName("Light1");
   Light2Tech =m_pFX->GetTechniqueByName("Light2");
   Light3Tech =m_pFX->GetTechniqueByName("Light3");

   Light0TexTech = m_pFX->GetTechniqueByName("Light0Tex");
   Light1TexTech = m_pFX->GetTechniqueByName("Light1Tex");
   Light2TexTech = m_pFX->GetTechniqueByName("Light2Tex");
   Light3TexTech = m_pFX->GetTechniqueByName("Light3Tex");

   Light0TexAlphaClipTech = m_pFX->GetTechniqueByName("Light0TexAlphaClip");
   Light1TexAlphaClipTech = m_pFX->GetTechniqueByName("Light1TexAlphaClip");
   Light2TexAlphaClipTech = m_pFX->GetTechniqueByName("Light2TexAlphaClip");
   Light3TexAlphaClipTech = m_pFX->GetTechniqueByName("Light3TexAlphaClip");

   Light1FogTech = m_pFX->GetTechniqueByName("Light1Fog");
   Light2FogTech = m_pFX->GetTechniqueByName("Light2Fog");
   Light3FogTech = m_pFX->GetTechniqueByName("Light3Fog");

   Light0TexFogTech = m_pFX->GetTechniqueByName("Light0TexFog");
   Light1TexFogTech = m_pFX->GetTechniqueByName("Light1TexFog");
   Light2TexFogTech = m_pFX->GetTechniqueByName("Light2TexFog");
   Light3TexFogTech = m_pFX->GetTechniqueByName("Light3TexFog");

   Light0TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipFog");
   Light1TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipFog");
   Light2TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipFog");
   Light3TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipFog");

   Light1ReflectTech = m_pFX->GetTechniqueByName("Light1Reflect");
   Light2ReflectTech = m_pFX->GetTechniqueByName("Light2Reflect");
   Light3ReflectTech = m_pFX->GetTechniqueByName("Light3Reflect");

   Light0TexReflectTech = m_pFX->GetTechniqueByName("Light0TexReflect");
   Light1TexReflectTech = m_pFX->GetTechniqueByName("Light1TexReflect");
   Light2TexReflectTech = m_pFX->GetTechniqueByName("Light2TexReflect");
   Light3TexReflectTech = m_pFX->GetTechniqueByName("Light3TexReflect");

   Light0TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipReflect");
   Light1TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipReflect");
   Light2TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipReflect");
   Light3TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipReflect");

   Light1FogReflectTech = m_pFX->GetTechniqueByName("Light1FogReflect");
   Light2FogReflectTech = m_pFX->GetTechniqueByName("Light2FogReflect");
   Light3FogReflectTech = m_pFX->GetTechniqueByName("Light3FogReflect");

   Light0TexFogReflectTech = m_pFX->GetTechniqueByName("Light0TexFogReflect");
   Light1TexFogReflectTech = m_pFX->GetTechniqueByName("Light1TexFogReflect");
   Light2TexFogReflectTech = m_pFX->GetTechniqueByName("Light2TexFogReflect");
   Light3TexFogReflectTech = m_pFX->GetTechniqueByName("Light3TexFogReflect");

   Light0TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipFogReflect");
   Light1TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipFogReflect");
   Light2TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipFogReflect");
   Light3TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipFogReflect");

   WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
   World = m_pFX->GetVariableByName("gWorld")->AsMatrix();
   WorldInvTranspose = m_pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
   TexTransform = m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
   EyePosW = m_pFX->GetVariableByName("gEyePosW")->AsVector();
   FogColor = m_pFX->GetVariableByName("gFogColor")->AsVector();
   FogStart = m_pFX->GetVariableByName("gFogStart")->AsScalar();
   FogRange = m_pFX->GetVariableByName("gFogRange")->AsScalar();
   DirLights = m_pFX->GetVariableByName("gDirLights");
   Mat = m_pFX->GetVariableByName("gMaterial");
   DiffuseMap = m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
   CubeMap = m_pFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}

//////////////////////////////////////////////////////////////////////////
NormalMapEffect::NormalMapEffect(ID3D11Device* device, const std::wstring& filename)
   : Effect(device, filename)
{
   Light1Tech = m_pFX->GetTechniqueByName("Light1");
   Light2Tech = m_pFX->GetTechniqueByName("Light2");
   Light3Tech = m_pFX->GetTechniqueByName("Light3");

   Light0TexTech = m_pFX->GetTechniqueByName("Light0Tex");
   Light1TexTech = m_pFX->GetTechniqueByName("Light1Tex");
   Light2TexTech = m_pFX->GetTechniqueByName("Light2Tex");
   Light3TexTech = m_pFX->GetTechniqueByName("Light3Tex");

   Light0TexAlphaClipTech = m_pFX->GetTechniqueByName("Light0TexAlphaClip");
   Light1TexAlphaClipTech = m_pFX->GetTechniqueByName("Light1TexAlphaClip");
   Light2TexAlphaClipTech = m_pFX->GetTechniqueByName("Light2TexAlphaClip");
   Light3TexAlphaClipTech = m_pFX->GetTechniqueByName("Light3TexAlphaClip");

   Light1FogTech = m_pFX->GetTechniqueByName("Light1Fog");
   Light2FogTech = m_pFX->GetTechniqueByName("Light2Fog");
   Light3FogTech = m_pFX->GetTechniqueByName("Light3Fog");

   Light0TexFogTech = m_pFX->GetTechniqueByName("Light0TexFog");
   Light1TexFogTech = m_pFX->GetTechniqueByName("Light1TexFog");
   Light2TexFogTech = m_pFX->GetTechniqueByName("Light2TexFog");
   Light3TexFogTech = m_pFX->GetTechniqueByName("Light3TexFog");

   Light0TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipFog");
   Light1TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipFog");
   Light2TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipFog");
   Light3TexAlphaClipFogTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipFog");

   Light1ReflectTech = m_pFX->GetTechniqueByName("Light1Reflect");
   Light2ReflectTech = m_pFX->GetTechniqueByName("Light2Reflect");
   Light3ReflectTech = m_pFX->GetTechniqueByName("Light3Reflect");

   Light0TexReflectTech = m_pFX->GetTechniqueByName("Light0TexReflect");
   Light1TexReflectTech = m_pFX->GetTechniqueByName("Light1TexReflect");
   Light2TexReflectTech = m_pFX->GetTechniqueByName("Light2TexReflect");
   Light3TexReflectTech = m_pFX->GetTechniqueByName("Light3TexReflect");

   Light0TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipReflect");
   Light1TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipReflect");
   Light2TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipReflect");
   Light3TexAlphaClipReflectTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipReflect");

   Light1FogReflectTech = m_pFX->GetTechniqueByName("Light1FogReflect");
   Light2FogReflectTech = m_pFX->GetTechniqueByName("Light2FogReflect");
   Light3FogReflectTech = m_pFX->GetTechniqueByName("Light3FogReflect");

   Light0TexFogReflectTech = m_pFX->GetTechniqueByName("Light0TexFogReflect");
   Light1TexFogReflectTech = m_pFX->GetTechniqueByName("Light1TexFogReflect");
   Light2TexFogReflectTech = m_pFX->GetTechniqueByName("Light2TexFogReflect");
   Light3TexFogReflectTech = m_pFX->GetTechniqueByName("Light3TexFogReflect");

   Light0TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light0TexAlphaClipFogReflect");
   Light1TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light1TexAlphaClipFogReflect");
   Light2TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light2TexAlphaClipFogReflect");
   Light3TexAlphaClipFogReflectTech = m_pFX->GetTechniqueByName("Light3TexAlphaClipFogReflect");

   WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
   World = m_pFX->GetVariableByName("gWorld")->AsMatrix();
   WorldInvTranspose = m_pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
   TexTransform = m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
   EyePosW = m_pFX->GetVariableByName("gEyePosW")->AsVector();
   FogColor = m_pFX->GetVariableByName("gFogColor")->AsVector();
   FogStart = m_pFX->GetVariableByName("gFogStart")->AsScalar();
   FogRange = m_pFX->GetVariableByName("gFogRange")->AsScalar();
   DirLights = m_pFX->GetVariableByName("gDirLights");
   Mat = m_pFX->GetVariableByName("gMaterial");
   DiffuseMap = m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
   CubeMap = m_pFX->GetVariableByName("gCubeMap")->AsShaderResource();
   NormalMap = m_pFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

NormalMapEffect::~NormalMapEffect()
{
}

///////////////////////////////////////////////////////////////////////////

SkinnedMeshEffect::SkinnedMeshEffect(ID3D11Device* device, const std::wstring& filename)
   : Effect(device, filename)
{
   Light1SkinnedTech = m_pFX->GetTechniqueByName("Light1Skinned");
   Light2SkinnedTech = m_pFX->GetTechniqueByName("Light2Skinned");
   Light3SkinnedTech = m_pFX->GetTechniqueByName("Light3Skinned");

   Light0TexSkinnedTech = m_pFX->GetTechniqueByName("Light0TexSkinned");
   Light1TexSkinnedTech = m_pFX->GetTechniqueByName("Light1TexSkinned");
   Light2TexSkinnedTech = m_pFX->GetTechniqueByName("Light2TexSkinned");
   Light3TexSkinnedTech = m_pFX->GetTechniqueByName("Light3TexSkinned");

   WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
   World = m_pFX->GetVariableByName("gWorld")->AsMatrix();
   WorldInvTranspose = m_pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
   BoneTransforms = m_pFX->GetVariableByName("gBoneTransforms")->AsMatrix();
   TexTransform = m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
   EyePosW = m_pFX->GetVariableByName("gEyePosW")->AsVector();
   DirLights = m_pFX->GetVariableByName("gDirLights");
   Mat = m_pFX->GetVariableByName("gMaterial");
   DiffuseMap = m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

SkinnedMeshEffect::~SkinnedMeshEffect()
{
}

///////////////////////////////////////////////////////////////////////////

SkyEffect::SkyEffect(ID3D11Device* device, const std::wstring& filename)
   : Effect(device, filename)
{
   SkyTech = m_pFX->GetTechniqueByName("SkyTech");
   WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
   CubeMap = m_pFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

SkyEffect::~SkyEffect()
{
}

//////////////////////////////////////////////////////////////////////////

BlurEffect::BlurEffect(ID3D11Device* pDevice, const std::wstring& filename)
	: Effect(pDevice, filename)
{
	HorzBlurTech = m_pFX->GetTechniqueByName("HorzBlur");
	VertBlurTech = m_pFX->GetTechniqueByName("VertBlur");

	Weights = m_pFX->GetVariableByName("gWeights")->AsScalar();
	InputMap = m_pFX->GetVariableByName("gInput")->AsShaderResource();
	OutputMap = m_pFX->GetVariableByName("gOutput")->AsUnorderedAccessView();
}

BlurEffect::~BlurEffect()
{
}

///////////////////////////////////////////////////////////////////////////////////////

BitmapEffect::BitmapEffect(ID3D11Device* device, const std::wstring& filename)
   : Effect(device, filename)
{
   WorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
   DiffuseMap = m_pFX->GetVariableByName("gShaderTexture")->AsShaderResource();
   LightTech = m_pFX->GetTechniqueByName("LightTech");
}

BitmapEffect::~BitmapEffect()
{
}

//////////////////////////////////////////////////////////////////////////

ColorEffect* StaticEffect::ColorFX = 0;
BasicEffect* StaticEffect::BasicFX = 0;
//NormalMapEffect* StaticEffect::NormalMapFX = 0;
//SkinnedMeshEffect* StaticEffect::SkinnedMeshFX = 0;
SkyEffect* StaticEffect::SkyFX = 0;
BlurEffect* StaticEffect::BlurFX = 0;
BitmapEffect* StaticEffect::BitmapFX = 0;

void StaticEffect::InitializeAll(ID3D11Device* pDevice)
{
	ColorFX = new ColorEffect(pDevice, L"../../Library/FX/color.cso");
	BasicFX = new BasicEffect(pDevice, L"../../Library/FX/Basic.cso");
   //NormalMapFX = new NormalMapEffect(pDevice, L"../../Library/FX/NormalMap.cso");
   //SkinnedMeshFX = new SkinnedMeshEffect(pDevice, L"../../Library/FX/SkinnedMesh.cso");
   SkyFX = new SkyEffect(pDevice, L"../../Library/FX/Sky.cso");
	BlurFX = new BlurEffect(pDevice, L"../../Library/FX/Blur.cso");
	BitmapFX = new BitmapEffect(pDevice, L"../../Library/FX/Bitmap.cso");
}

void StaticEffect::FinalizeAll()
{
	SafeDelete(ColorFX);
	SafeDelete(BasicFX);
	//SafeDelete(NormalMapFX);
	//SafeDelete(SkinnedMeshFX);
   SafeDelete(SkyFX);
	SafeDelete(BlurFX);
	SafeDelete(BitmapFX);
}
