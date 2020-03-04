//step4: note that there is no "main" any more..that means you need to change the entry point name to VS in HLSL compiler!
cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 Color : COLOR;
};



//It is possible to have the input assembler generate a vertex ID. 
//To do this, add an additional parameter of type uint to the vertex shader signature 
//with semantic SV_VertexID: VertexOut VS(VertexIn vin, uint vertID : SV_VertexID)


VertexOut VS(VertexIn vin, uint vertID : SV_VertexID)
{
    VertexOut vout;

	// Just pass data over to geometry shader.

    vout.PosW = float4(vin.PosL, 1.0f);

	// Just pass vertex color into the pixel shader.
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if(vertID % 3 == 0  ) 
    color = float4(1.0f, 0.0f, 0.0f, 1.0f);

    if (vertID % 3 == 1) 
        color = float4(0.0f, 1.0f, 0.0f, 1.0f);

    if (vertID % 3 == 2) 
        color = float4(0.0f, 0.0f, 1.0f, 1.0f);

    //vout.Color = vin.Color;
    vout.Color = color;

    return vout;
}