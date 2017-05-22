
struct PSIn
{
    float4 Position : SV_Position;
    float4 DepthPosition : POSITION;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float depth = input.DepthPosition.z / input.DepthPosition.w;
    return float4(depth, depth, depth, 1.0f);
}