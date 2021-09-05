
// ��Ʈ�� �̹��� ����

cbuffer cbPerFrame
{
   float4x4 gWorldViewProj;
};

Texture2D gShaderTexture : register(t0);

SamplerState samLinear
{
   Filter = MIN_MAG_MIP_LINEAR;
   AddressU = WRAP;
   AddressV = WRAP;
};

BlendState AlphaBlend
{
    // �׵θ� ó�� False�� ��
   AlphaToCoverageEnable = false;

    // ����ó��
   BlendEnable[0] = true; // ���� ��� ���, 8���� �̹������� ��� ����

    /*
    ���� �Լ�

    INV_SRC_ALPHA : �ҽ� �ؽ����� ���ĸ� ������Ų ��
    ��, 1���� �ҽ� �ؽ����� ���ĸ�ŭ �� ��.

    ex) ���� �ҽ��� ���İ� 0.3�̶�� ��ǥ�� ���Ĵ� 0.7�� ���� ��ǥ �ȼ��� 70%�� ���
    */

   DestBlend[0] = INV_SRC_ALPHA; // �̹� �� �ڸ��� �׷��� �ִ� ��ǥ �ȼ��� ����
   SrcBlend[0] = SRC_ALPHA; // �ҽ� �ؽ����� ������ ����ϴµ� ���, (���� �Լ�)SRC_ALPHA : �ؽ��İ� �����ִ� ���İ� �״�λ��
   BlendOp[0] = Add; // ���� ���, Add : ���ؼ� ���� �ȼ� ����

   SrcBlendAlpha[0] = One; // �� �� ���ڴ�.
   DestBlendAlpha[0] = One; // �� �� ���ڴ�.
   RenderTargetWriteMask[0] = 0x0f;
};


struct VertexIn
{
   float3 PosL : POSITION;
   float2 Tex : TEXCOORD;
};

struct VertexOut
{
   float4 PosH : SV_POSITION;
   float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
   VertexOut vout = (VertexOut) 0;
 
   vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

   vout.Tex = vin.Tex;
   
   vout.PosH.z = 0.0f;

   return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
   float4 textureColor;
    
   textureColor = gShaderTexture.Sample(samLinear, pin.Tex);
    
   return textureColor;
}

technique11 LightTech
{
   pass P0
   {
      SetVertexShader(CompileShader(vs_5_0, VS()));
      SetPixelShader(CompileShader(ps_5_0, PS()));

      SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF); // 0xFFFFFFFF = 0xFF �˾Ƽ� ���� ����־���

   }
}