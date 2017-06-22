
cbuffer cbPerObject : register(b0)
{
	float4x4 g_CamWVP;
	float4x4 g_World;
};

cbuffer cbPerLight : register(b1)
{
	float4x4 g_LightView;
	float4x4 g_LightProjection;
};

cbuffer cbPerFrame : register(b2)
{
	float3 g_CamPos;
};

struct VSOut
{
	float4 Position : SV_Position;
	float4 LightViewPosition : POSITION0;
	float3 VertexToCamVector : POSITION1;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEXCOORD;
};

VSOut main(float4 inPos : POSITION, float3 inNor : NORMAL, float2 inTex : TEXCOORD, float3 inTag : TANGENT, float3 inBin : BINORMAL)
{
	VSOut output = (VSOut) 0;

	float4 WorldPos = mul(inPos, g_World);

	output.Position = mul(inPos, g_CamWVP);

	output.LightViewPosition = mul(WorldPos, g_LightView);
	output.LightViewPosition = mul(output.LightViewPosition, g_LightProjection);
	
	
	output.Normal = mul(inNor, (float3x3) g_World);
	output.Normal = normalize(output.Normal);

	output.Tangent = mul(inTag, (float3x3) g_World);
	output.Tangent = normalize(output.Tangent);

	output.Binormal = mul(inBin, (float3x3) g_World);
	output.Binormal = normalize(output.Binormal);

	output.TexCoord = inTex;

	return output;
}