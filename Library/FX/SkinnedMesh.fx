//=============================================================================
// NormalMap.fx by Frank Luna (C) 2011 All Rights Reserved.
//=============================================================================

#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
   DirectionalLight gDirLights[3];
   float3 gEyePosW;
};

cbuffer cbPerObject
{
   float4x4 gWorld;
   float4x4 gWorldInvTranspose;
   float4x4 gWorldViewProj;
   float4x4 gTexTransform;
   Material gMaterial;
}; 

cbuffer cbSkinned
{
   float4x4 gBoneTransforms[96];
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samLinear
{
   Filter = MIN_MAG_MIP_LINEAR;
   AddressU = WRAP;
   AddressV = WRAP;
};

struct SkinnedVertexIn
{
   float3 PosL : POSITION;
   float3 NormalL : NORMAL;
   float2 Tex : TEXCOORD;
   float3 Weights : WEIGHTS;
   uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
   float4 PosH : SV_POSITION;
   float3 PosW : POSITION;
   float3 NormalW : NORMAL;
   float2 Tex : TEXCOORD0;
};

VertexOut SkinnedVS(SkinnedVertexIn vin)
{
   VertexOut vout;

	// Init array or else we get strange warnings about SV_POSITION.
	// 버텍스 가중치 배열을 초기화한다. 아니면 SV_POSITION 값에 대한 이상한 워닝을 볼 것이다.
   float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   weights[0] = vin.Weights.x;
   weights[1] = vin.Weights.y;
   weights[2] = vin.Weights.z;
   weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

   float3 posL = float3(0.0f, 0.0f, 0.0f);
   float3 normalL = float3(0.0f, 0.0f, 0.0f);

   for (int i = 0; i < 4; ++i)
   {
	    // Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.
		// 논유니폼 스케일이 없다고 가정하므로, 노말값을 위한 역행렬의 전치행렬이 필요없다.
      posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
      normalL += weights[i] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndices[i]]);
   }
 
	// Transform to world space space.
	// 월드 스페이스로 변환
   vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
   vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose); // 이때는 역전치행렬을 쓴다.

	// Transform to homogeneous clip space.
   vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
   vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

   return vout;
}
 
float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
   pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
   float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
   float distToEye = length(toEye);

	// Normalize.
   toEye /= distToEye;
	
    // Default to multiplicative identity.
   float4 texColor = float4(1, 1, 1, 1);
   if (gUseTexure)
   {
		// Sample texture.
      texColor = gDiffuseMap.Sample(samLinear, pin.Tex);
   }

	// Lighting.
   float4 litColor = texColor;
   if (gLightCount > 0)
   {
		// Start with a sum of zero. 
      float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
      float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
      float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
		  
		// Sum the light contribution from each light source.  
		[unroll] 
      for (int i = 0; i < gLightCount; ++i)
      {
         float4 A, D, S;
         ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
				A, D, S);

         ambient += A;
         diffuse += D;
         spec += S;
      }
		   
      litColor = texColor * (ambient + diffuse) + spec;
   }
 
	// Common to take alpha from diffuse material and texture.
   litColor.a = gMaterial.Diffuse.a * texColor.a;

   return litColor;
}

technique11 Light1Skinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(1, false)));
   }
}

technique11 Light2Skinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(2, false)));
   }
}

technique11 Light3Skinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
   }
}

technique11 Light0TexSkinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(0, true)));
   }
}

technique11 Light1TexSkinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
   }
}

technique11 Light2TexSkinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
   }
}

technique11 Light3TexSkinned
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
      SetGeometryShader(NULL);
      SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
   }
}
