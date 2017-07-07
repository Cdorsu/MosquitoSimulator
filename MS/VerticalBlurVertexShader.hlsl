

cbuffer cbPerObject : register(b0)
{
	float4x4 WVP; // Ortographic
	float TextureHeight;
}

struct VSOut
{
	float4 Position : SV_Position;
	float2 Texture : TEXCOORD;
	float2 Coords[11] : TEXCOORD1;
};

VSOut main(float4 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut output = (VSOut) 0;

	output.Position = mul(pos, WVP);
	output.Texture = tex;

	float pixelHeight = 1.0f / TextureHeight;

	[unroll]
	for (int i = -5; i <= 5; ++i)
	{
		output.Coords[i + 5] = tex + float2(0.0f, i * pixelHeight);
	}

	return output;
}