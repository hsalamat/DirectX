//If a geometry shader is no present, the primitive ID parameter can be added to
//the parameter list of the pixelshader:
//float4 PS(VertexOut pin, uint primID : SV_PrimitiveID) : SV_Target

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    uint PrimID : SV_PrimitiveID;
};

//float4 PS(VertexOut pin) : SV_Target
float4 PS(VertexOut pin, uint primID : SV_PrimitiveID) : SV_Target
{
    float4 color = (1.0f, 1.0f, 1.0f, 1.0f);
    if (primID == 0) 
        color = float4(1.0f, 0.0f, 0.0f, 1.0f);

    if (primID == 1) 
        color = float4(0.0f, 1.0f, 0.0f, 1.0f);

    if (primID == 2) 
        color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    //return pin.Color;
    return color;
}








