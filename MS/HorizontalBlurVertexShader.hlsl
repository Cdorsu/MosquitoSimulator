
cbuffer cbPerObject
{
    float4x4 WVP; // Only ortographic
    float Width;
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

VSOut main( float4 inPos : POSITION, float2 inTex : TEXCOORD )
{
    VSOut output = (VSOut) 0;

    output.Position = mul(inPos, WVP);
    output.Texture = inTex;

    float texelWidth = (1.0f / Width);

    output.Texture1 = inTex + float2(-4 * texelWidth, 0.0f);
    output.Texture2 = inTex + float2(-3 * texelWidth, 0.0f);
    output.Texture3 = inTex + float2(-2 * texelWidth, 0.0f);
    output.Texture4 = inTex + float2(-1 * texelWidth, 0.0f);
    output.Texture5 = inTex + float2(+0 * texelWidth, 0.0f);
    output.Texture6 = inTex + float2(+1 * texelWidth, 0.0f);
    output.Texture7 = inTex + float2(+2 * texelWidth, 0.0f);
    output.Texture8 = inTex + float2(+3 * texelWidth, 0.0f);
    output.Texture9 = inTex + float2(+4 * texelWidth, 0.0f);

    return output;
}