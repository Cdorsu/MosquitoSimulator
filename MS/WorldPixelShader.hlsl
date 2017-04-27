
Texture2D ObjTexture : register(t0);

SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

cbuffer cbLight : register( b0 )
{
    float3 Direction;
    float4 Diffuse;
    float4 Ambient;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 textureColor = ObjTexture.Sample(WrapSampler, input.TexCoord);
    float4 color = Ambient;
    float howMuchLight = saturate ( dot ( input.Normal, -Direction ) );
    if ( howMuchLight > 0.0f )
    {
        color += howMuchLight * Diffuse;
    }
    return color * textureColor;
}