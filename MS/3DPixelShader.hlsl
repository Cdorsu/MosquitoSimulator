
Texture2D ObjTexture : register(t0);
SamplerState ObjSampler : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4 g_Color;
    bool bHasTexture;
};

struct PSIn
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    if (bHasTexture)
        Color = ObjTexture.Sample(ObjSampler, input.Texture);
    else
        Color = g_Color;
    clip(Color.a - 0.2f);
    return Color;
}