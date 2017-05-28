

cbuffer cbPerObject : register(b0)
{
    float4x4 WVP;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
};

VSOut main( float4 pos : POSITION, float2 inTex : TEXCOORD )
{
    VSOut output = (VSOut) 0;
    output.Position = mul(pos, WVP);
    output.Texture = inTex;
    return output;
}