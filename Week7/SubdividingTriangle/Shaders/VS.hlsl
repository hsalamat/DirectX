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

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Transform to homogeneous clip space.

	// Just pass data over to geometry shader.

    //vout.PosW = float4(vin.PosL, 1.0f);
    vout.PosW = vin.PosL;

	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;

    return vout;
}