
Texture2D ObjTexture : register(t0);
Texture2D ObjSecondTexture : register(t1);
SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 color = ObjTexture.Sample(WrapSampler, input.TexCoord)
            + ObjSecondTexture.Sample(WrapSampler, input.TexCoord);
    if (color.a == 0.0f ||
        (color.r == 0 && color.g == 0 && color.b == 0))
        discard;
    return color;
}