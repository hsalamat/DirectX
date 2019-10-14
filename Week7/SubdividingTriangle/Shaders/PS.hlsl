struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 Color : COLOR;
   // uint PrimID : SV_PrimitiveID;
};

float4 PS(GeoOut pin) : SV_Target
{
    return pin.Color;
}