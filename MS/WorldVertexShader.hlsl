
cbuffer cbMatrices : register(b0)
{
    float4x4 WVP;
    float4x4 World;
    float3 camPos;
};

struct VSOut
{
    float4 Position : SV_Position;
    float3 worldPos : POSITION0;
    float3 VertexToCamVector : POSTION1;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

VSOut main(float4 inPos : POSITION, float2 inTex : TEXCOORD, float3 inNor : NORMAL)
{
    VSOut output = (VSOut) 0;

    output.Position = mul(inPos, WVP);
    output.worldPos = mul(inPos, World).xyz;
    output.TexCoord = inTex;

    output.Normal = mul ( inNor, ( float3x3 ) World );
    output.Normal = normalize ( output.Normal );

    output.VertexToCamVector = camPos - output.worldPos;
    output.VertexToCamVector = normalize(output.VertexToCamVector);

    return output;
}