
Texture2D ObjTexture : register(t0);
SamplerState ClampSampler : register(s0);


struct PSIn
{
	float4 Position : SV_Position;
	float2 Texture : TEXCOORD;
	float2 Coords[11] : TEXCOORD1;
};

static const float weights[11] =
{
	0.0093f,
	0.028002f,
	0.065984f,
	0.121703f,
	0.175713f,
	0.198596f,
	0.175713f,
	0.121703f,
	0.065984f,
	0.028002f,
	0.0093f,

};

float4 main(PSIn input) : SV_TARGET
{
	float4 Color = float4(0, 0, 0, 0);

	[unroll]
	for (int i = 0; i < 11; ++i)
	{
		Color += ObjTexture.Sample(ClampSampler, input.Coords[i]) * weights[i];
	}

	return Color;
}