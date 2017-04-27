
Texture2D ObjTexture : register(t0);
Texture2D ObjSpecularMap : register(t1);

SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float3 worldPos : POSITION0;
    float3 PixelToCam : POSTION1;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

cbuffer cbLight : register( b0 )
{
    float3 Direction;
    float SpecularPower;
    float4 Diffuse;
    float4 Ambient;
    float4 SpecularColor;
};

float4 GetSpecular(float lightIntensity, float specPower, float3 normal, float3 lightDir,
        float3 PixelToCam, float4 specularColor, float4 specularIntensity)
{
    float3 reflection = normalize(2 * lightIntensity * normal - lightDir);
    float angle = dot(PixelToCam, lightDir);
    float4 specular = saturate(pow(angle, specPower));
    specular *= specularIntensity;
    return specular * specularColor;
}

float4 main(PSIn input) : SV_TARGET
{
    float4 textureColor = ObjTexture.Sample(WrapSampler, input.TexCoord);
    float4 specularIntensity = ObjSpecularMap.Sample(WrapSampler, input.TexCoord);
    float4 color = Ambient;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float howMuchLight = saturate ( dot ( input.Normal, -Direction ) );
    if ( howMuchLight > 0.0f )
    {
        color += howMuchLight * Diffuse;
        color = saturate(color);
        specular = GetSpecular(howMuchLight, SpecularPower, input.Normal,
         -Direction, input.PixelToCam, SpecularColor, specularIntensity);
    }
    color = color * textureColor;
    color = saturate(color + specular);
    return color;
}