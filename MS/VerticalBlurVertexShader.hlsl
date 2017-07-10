
cbuffer cbPerObject
{
    float4x4 WVP; // Only ortographic
    float Height;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
    float2 Texture1 : TEXCOORD1;
    float2 Texture2 : TEXCOORD2;
    float2 Texture3 : TEXCOORD3;
    float2 Texture4 : TEXCOORD4;
    float2 Texture5 : TEXCOORD5;
    float2 Texture6 : TEXCOORD6;
    float2 Texture7 : TEXCOORD7;
    float2 Texture8 : TEXCOORD8;
    float2 Texture9 : TEXCOORD9;
};

VSOut main(float4 inPos : POSITION, float2 inTex : TEXCOORD)
{
    VSOut output = (VSOut) 0;

    output.Position = mul(inPos, WVP);
    output.Texture = inTex;

    float texelHeight = (1.0f / Height);

    output.Texture1 = inTex + float2(0.0f, -4 * texelHeight);
    output.Texture2 = inTex + float2(0.0f, -3 * texelHeight);
    output.Texture3 = inTex + float2(0.0f, -2 * texelHeight);
    output.Texture4 = inTex + float2(0.0f, -1 * texelHeight);
    output.Texture5 = inTex + float2(0.0f, +0 * texelHeight);
    output.Texture6 = inTex + float2(0.0f, +1 * texelHeight);
    output.Texture7 = inTex + float2(0.0f, +2 * texelHeight);
    output.Texture8 = inTex + float2(0.0f, +3 * texelHeight);
    output.Texture9 = inTex + float2(0.0f, +4 * texelHeight);

    return output;
}