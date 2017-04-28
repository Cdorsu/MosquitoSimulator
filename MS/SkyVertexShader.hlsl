
cbuffer cbMatrices
{
    float4x4 WVP;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 TexCoord : TEXCOORD;
};

VSOut main( float3 pos : POSITION )
{
    VSOut output = (VSOut) output;
    output.Position = mul(float4(pos, 1.0f), WVP).xyww;
    output.TexCoord = pos.xyz;
    return output;
}