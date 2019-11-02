//***************************************************************************************
// Tessellation2.hlsl Triangle Tessellation
//use right mouse button to zoom in and out to see different tessellation based on distance
//***************************************************************************************

#include "VS.hlsl"


struct PatchTess
{
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess[1] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
    float3 centerL = 0.333f * (patch[0].PosL + patch[1].PosL + patch[2].PosL);
    float3 centerW = mul(float4(centerL, 1.0f), gWorld).xyz;

    float d = distance(centerW, gEyePosW);

// Tessellate the patch based on distance from the eye such that
// the tessellation is 0 if d >= d1 and 33 if d <= d0.  The interval
// [d0, d1] defines the range we tessellate in.

    const float d0 = 1.0f;
    const float d1 = 25.0f;
    float tess = 33.0f * saturate((d1 - d) / (d1 - d0));

// Uniformly tessellate the patch.

    pt.EdgeTess[0] = tess; // Left edge
    pt.EdgeTess[1] = tess; // Right edge
    pt.EdgeTess[2] = tess; // Bottom edge
    pt.InsideTess[0] = tess; // inside

    return pt;

}

struct HullOut
{
	float3 PosL : POSITION;
    float4 Color : COLOR;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p, 
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
[domain("tri")]
DomainOut DS(PatchTess patchTess, 
             float3 uvw : SV_DomainLocation, 
             const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout;
	

    float3 p = tri[0].PosL * uvw.x + tri[1].PosL * uvw.y + tri[2].PosL * uvw.z;
	
	float4 posW = mul(float4(p, 1.0f), gWorld);
	dout.PosH = mul(posW, gViewProj);
    dout.Color = tri[0].Color;
	return dout;
}




