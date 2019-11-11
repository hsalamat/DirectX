//***************************************************************************************
// Tessellation.hlsl 
//***************************************************************************************

#include "VS.hlsl"



struct PatchTess
{
	float EdgeTess[2]   : SV_TessFactor;
	//float InsideTess[2] : SV_TessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
    // Uniformly tessellate the patch 3 times.
    pt.EdgeTess[0] = 8; // Left edge
    pt.EdgeTess[1] = 8; // Top edge
   // pt.EdgeTess[2] = 2; // Right edge
   // pt.EdgeTess[3] = 2; // Bottom edge
   // pt.InsideTess[0] = 3; // u-axis (columns)
    //pt.InsideTess[1] = 3; // v-axis (rows)
    return pt;

	
	return pt;
}

struct HullOut
{
	float3 PosL : POSITION;
    float4 Color : COLOR;
};

[domain("isoline")]
[partitioning("integer")]  //change it to fractional_odd or fractional_even
[outputtopology("line")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p, 
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	hout.PosL = p[i].PosL;
    hout.Color = p[i].Color;
	
	return hout;
}

struct DomainOut
{
	float4 PosH : SV_POSITION;
    float4 Color : Color;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("isoline")]
DomainOut DS(PatchTess patchTess, 
             float2 uv : SV_DomainLocation, 
             const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;
	
	// Bilinear interpolation.
	float3 v1 = lerp(quad[0].PosL, quad[1].PosL, uv.x); 
	float3 v2 = lerp(quad[2].PosL, quad[3].PosL, uv.x); 
	float3 p  = lerp(v1, v2, uv.y); 
	
	// Displacement mapping
	//p.y = 0.3f*( p.z*sin(p.x) + p.x*cos(p.z) );
	
	float4 posW = mul(float4(p, 1.0f), gWorld);
	dout.PosH = mul(posW, gViewProj);
    dout.Color = quad[0].Color;
	return dout;
}




