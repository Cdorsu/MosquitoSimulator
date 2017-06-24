
cbuffer cbMatrices : register(b0)
{
    float4x4 WVP; // Ortho
}

struct VSOut
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

VSOut main(float4 inPos : POSITION, float2 inTex : TEXCOORD)
{
    VSOut output = (VSOut) 0;
    output.Position = mul(inPos, WVP);
    output.TexCoord = inTex;
    return output;
}