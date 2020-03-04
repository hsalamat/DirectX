struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 Color : COLOR;
    uint PrimID : SV_PrimitiveID;
};

float4 PS(GeoOut pin) : SV_Target
{
    //you can't have access to primitive id here, since you have a geometry shader!
    if (pin.PrimID == 0)
        return float4(1.0, 1.0, 0.0, 1.0);
    return pin.Color;
}