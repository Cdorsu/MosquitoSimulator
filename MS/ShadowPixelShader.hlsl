
Texture2D ObjTexture : register( t0 );
Texture2D ObjDepthmap : register( t1 );
Texture2D ObjSpecularmap : register(t2);
Texture2D ObjNormalmap : register(t3);

SamplerState WrapSampler : register( s0 );
SamplerState ClampSampler : register( s1 );

cbuffer cbLight : register( b0 )
{
	float4 Diffuse;
	float4 Ambient;
    float4 SpecularColor;
};

cbuffer cbMaterialInfo : register(b1)
{
    float4 Color;
    bool bHasTexture;
    bool bHasSpecularMap;
    bool bHasNormalMap;
    float SpecularPower;
};

struct PSIn
{
    float4 Position : SV_Position;
    float4 LightViewPosition : POSITION0;
    float3 VertexToLightVector : POSITION1;
    float3 VertexToCamVector : POSITION2;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEXCOORD;
};

float4 GetBumpColor(float3 Tangent, float3 Binormal, float3 Normal,
            float3 LightDir, float4 bumpmapColor, float4 Ambient)
{
    float3 bumpNormal;
    float lightIntensity;
    bumpmapColor = (bumpmapColor * 2.0f) - 1.0f;
    bumpNormal = bumpmapColor.x * Tangent + bumpmapColor.y * Binormal
                + bumpmapColor.z * Normal;
    bumpNormal = normalize(bumpNormal);
    lightIntensity = saturate(dot(bumpNormal, LightDir));
    return lightIntensity * Ambient;
}

float4 main ( PSIn input) : SV_TARGET
{
    float4 TextureColor;
    float4 BumpmapColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    [flatten]
    if (bHasTexture)
    {
        TextureColor = ObjTexture.Sample(WrapSampler, input.TexCoord);
        if(bHasNormalMap)
        {
            float4 BC = ObjNormalmap.Sample(WrapSampler, input.TexCoord);
            BumpmapColor = GetBumpColor(input.Tangent, input.Binormal,
             input.Normal, input.VertexToLightVector, BC, Ambient);
        }
    }
    else
    {
        TextureColor = Color;
    }
    float4 Color = Ambient;
	float2 projTexCoord;
    float4 Specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
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
                float3 v = reflect(-input.VertexToLightVector, input.Normal);
                float lightIntensity = saturate(dot(v, input.VertexToCamVector));
                Specular = pow(lightIntensity, SpecularPower);
                Specular *= SpecularColor;
                if (bHasSpecularMap)
                    Specular *= ObjSpecularmap.Sample(WrapSampler, input.TexCoord);
            }
		}
	}
    Color = saturate(BumpmapColor + Color);
    Color = Color * TextureColor;
    Color = saturate(Color + Specular);
    return Color;
}