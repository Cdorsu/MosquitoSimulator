
Texture2D ObjTexture : register(t0);
SamplerState ClampSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
    float2 Texture1 : TEXCOORD1;
    float2 Texture2 : TEXCOORD2;
    float2 Texture3 : TEXCOORD3;
    float2 Texture4 : TEXCOORD4;
    float2 Texture5 : TEXCOORD5;
    float2 Texture6 : TEXCOORD6;
    float2 Texture7 : TEXCOORD7;
    float2 Texture8 : TEXCOORD8;
    float2 Texture9 : TEXCOORD9;
};

static float weight4 = 0.0162162162;
static float weight3 = 0.0540540541;
static float weight2 = 0.1216216216;
static float weight1 = 0.1945945946;
static float weight0 = 0.2270270270;

float4 main(PSIn input) : SV_TARGET
{
    float4 Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Color += ObjTexture.Sample(ClampSampler, input.Texture1) * weight4;
    Color += ObjTexture.Sample(ClampSampler, input.Texture2) * weight3;
    Color += ObjTexture.Sample(ClampSampler, input.Texture3) * weight2;
    Color += ObjTexture.Sample(ClampSampler, input.Texture4) * weight1;
    Color += ObjTexture.Sample(ClampSampler, input.Texture5) * weight0;
    Color += ObjTexture.Sample(ClampSampler, input.Texture6) * weight1;
    Color += ObjTexture.Sample(ClampSampler, input.Texture7) * weight2;
    Color += ObjTexture.Sample(ClampSampler, input.Texture8) * weight3;
    Color += ObjTexture.Sample(ClampSampler, input.Texture9) * weight4;
    return Color;
}