
Texture2D ObjTexture : register(t0);

SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 textureColor = ObjTexture.Sample(WrapSampler, input.TexCoord);
    return textureColor;
}