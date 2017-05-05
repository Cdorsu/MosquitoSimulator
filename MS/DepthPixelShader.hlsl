
Texture2D ObjTexture : register(t0);
SamplerState ObjWrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float4 DepthPosition : POSITION;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 color = ObjTexture.Sample(ObjWrapSampler, input.TexCoord);
    if (color.a < 0.3f)
        discard;
    return input.DepthPosition.z / input.DepthPosition.w;
}