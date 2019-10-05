//***************************************************************************************
// PS_Anisotropic.hlsl 
//
// Default shader, currently supports lighting.
//***************************************************************************************

#include "VS_Anisotropic.hlsl"

float4 PS(VertexOut pin) : SV_Target
{
    //step2: use gsamMirror
    float4 diffuseAlbedo = gDiffuseMap.Sample(gsamAnisotropicWrap, pin.TexC) * gDiffuseAlbedo;

    //float diffuseAlbedo2 = float4(0.9f, 0.9f, 1.0f, 1.0f);
    //float4 diffuseAlbedo = gDiffuseMap.Sample(gsamLinear, pin.TexC) * diffuseAlbedo2;

    // Interpolating normal can unnormalize it, so renormalize it.
    pin.NormalW = normalize(pin.NormalW);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(gEyePosW - pin.PosW);

    // Light terms. Note that we are using diffuseAlbedo instead of gDiffuseAlbedo
    float4 ambient = gAmbientLight*diffuseAlbedo;

    const float shininess = 1.0f - gRoughness;
    Material mat = { diffuseAlbedo, gFresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 directLight = ComputeLighting(gLights, mat, pin.PosW,
        pin.NormalW, toEyeW, shadowFactor);

    float4 litColor = ambient + directLight;

    // Common convention to take alpha from diffuse material.
    litColor.a = diffuseAlbedo.a;

    return litColor;
}


