
TextureCube ObjTexture : register(t0);
SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float3 TexCoord : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    return ObjTexture.Sample(WrapSampler, input.TexCoord);
}