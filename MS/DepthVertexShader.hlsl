
cbuffer cbMatrices : register(b0)
{
    float4x4 WVP;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 DepthPosition : POSITION;
    float2 TexCoord : TEXCOORD;
};

VSOut main(float4 inPos : POSITION, float2 inTex : TEXCOORD)
{
    VSOut output = (VSOut) 0;

    output.Position = mul(inPos, WVP);
    output.DepthPosition = output.Position;
    output.TexCoord = inTex;

    return output;
}