//step4: note that there is no "main" any more..that means you need to change the entry point name to VS in HLSL compiler!
cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 PosW : POSITION;
    float4 Color : COLOR;
    float2 SizeW : SIZE;
};

struct VertexOut
{
    float3 CenterW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Just pass data over to geometry shader.
    vout.CenterW = vin.PosW;
    vout.SizeW = vin.SizeW;

	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;

    return vout;
}