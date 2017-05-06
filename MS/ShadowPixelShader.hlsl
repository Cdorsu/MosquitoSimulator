
Texture2D ObjTexture : register( t0 );
Texture2D ObjDepthmap : register( t1 );

SamplerState WrapSampler : register( s0 );
SamplerState ClampSampler : register( s1 );

cbuffer cbLight : register( b0 )
{
	float4 Diffuse;
	float4 Ambient;
};

struct PSIn
{
	float4 Position : SV_Position;
	float4 LightViewPosition : POSITION0;
	float3 VertexToLightVector : POSITION1;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

float4 main ( PSIn input) : SV_TARGET
{
	float4 TextureColor = ObjTexture.Sample ( WrapSampler, input.TexCoord );
	float4 Color = Ambient;
	float2 projTexCoord;
	float bias = 0.005f;

	projTexCoord.x = input.LightViewPosition.x / input.LightViewPosition.w / 2.0f + 0.5f;
	projTexCoord.y = -input.LightViewPosition.y / input.LightViewPosition.w / 2.0f + 0.5f;

	if ( projTexCoord.x == saturate ( projTexCoord.x ) && projTexCoord.y == saturate ( projTexCoord.y ))
	{
		float depthToCompare = ObjDepthmap.Sample ( ClampSampler, projTexCoord ).r;
		float depth = input.LightViewPosition.z / input.LightViewPosition.w;
		depth -= bias;
		if ( depth < depthToCompare )
		{
			float howMuchLight = saturate ( dot ( input.VertexToLightVector, input.Normal ) );
			if ( howMuchLight > 0.0f )
			{
				Color += Diffuse * howMuchLight;
			}
		}
	}
	Color = Color * TextureColor;
	return Color;
}