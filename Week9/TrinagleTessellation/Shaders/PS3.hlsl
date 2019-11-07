
struct DomainOut
{
    float4 PosH : SV_POSITION;
    float4 Color : Color;
};

float4 PS(DomainOut pin) : SV_Target
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    //return pin.Color;
}