
cbuffer cbMatrices : register(b0)
{
    float4x4 WVP;
    float4x4 World;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

VSOut main(float4 inPos : POSITION, float2 inTex : TEXCOORD, float3 inNor : NORMAL)
{
    VSOut output = (VSOut) 0;

    output.Position = mul(inPos, WVP);
    output.TexCoord = inTex;

    output.Normal = mul ( inNor, ( float3x3 ) World );
    output.Normal = normalize ( output.Normal );

    return output;
}