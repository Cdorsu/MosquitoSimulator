
Texture2D ObjTexture : register(t0);
Texture2D ObjSpecularMap : register(t1);
Texture2D ObjBumpMap : register( t2 );

SamplerState WrapSampler : register(s0);

struct PSIn
{
    float4 Position : SV_Position;
    float3 worldPos : POSITION0;
    float3 PixelToCamVector : POSTION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
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
    float angle = saturate(dot(PixelToCam, lightDir));
    float4 specular = pow(angle, specPower);
    specular *= specularIntensity;
    return specular * specularColor;
}

float4 GetBumpColor ( float3 Tangent, float3 Binormal, float3 Normal,
            float3 LightDir, float4 bumpmapColor, float4 Ambient )
{
    float3 bumpNormal;
    float lightIntensity;
    bumpmapColor = ( bumpmapColor * 2.0f ) - 1.0f;
    bumpNormal = bumpmapColor.x * Tangent + bumpmapColor.y * Binormal
                + bumpmapColor.z * Normal;
    bumpNormal = normalize ( bumpNormal );
    lightIntensity = saturate ( dot ( bumpNormal, LightDir ) );
    return lightIntensity * Ambient;
}

float4 main(PSIn input) : SV_TARGET
{
    float4 textureColor = ObjTexture.Sample ( WrapSampler, input.TexCoord );
    float4 specularColor = ObjSpecularMap.Sample ( WrapSampler, input.TexCoord );
    float4 bumpmapColor = ObjBumpMap.Sample ( WrapSampler, input.TexCoord );
    float4 color = float4 ( 0.0f, 0.0f, 0.0f, 0.0f ), specular = float4 ( 0.0f, 0.0f, 0.0f, 0.0f );
    float4 BumpColor = GetBumpColor ( input.Tangent, input.Binormal,
                         input.Normal, -Direction, bumpmapColor, Ambient ) * 2.0f;
    float lightIntensity = saturate ( dot ( input.Normal, -Direction ) );
    if ( lightIntensity > 0.0f )
    {
        
        color = saturate ( Diffuse * lightIntensity );
        specular = GetSpecular ( lightIntensity, SpecularPower, input.Normal, -Direction,
            input.PixelToCamVector, SpecularColor, specularColor );

    }
    color = saturate ( BumpColor + color );
    color = color * textureColor;
    color = saturate ( color + specular );
    return color;
}