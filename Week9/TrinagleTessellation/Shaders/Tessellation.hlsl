//***************************************************************************************
// Tessellation.hlsl 
//The triangle vertices were processed by the VS. 
//The Control Point Hull Shader gets a triangle as a patch with 3 CPs and simply passed it through to the Domain Shader. 
//The Constant Hull Shader sets the edge and inside tessellation factors.
//The Tessellation stage subdivides an equilateral triangle into smaller triangles and 
//execute the Domain Shader for every generated vertex. In each Domain Shader invocation we can access 
//the barycentric coordinates (a.k.a Tessellation Coordinates) of the vertex in the 3D-float uvw. 
//Since the barycentric coordinates within a triangle represents a weight combination of the 3 vertices,
//we can use it to interpolate all the attributes of the new vertex. 
// The world space position for each new vertex is calculated. 
// The homogenous position of new matrix is calculated by multiplying the world space position by view projection matrix
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
	
    // Uniformly tessellate the patch 4 times.   
    pt.EdgeTess[0] = 4; // Left edge
    pt.EdgeTess[1] = 4; // Right edge
    pt.EdgeTess[2] = 4; // Bottom edge
    pt.InsideTess[0] = 4; // inside

    //pt.EdgeTess[0] = 1; // Left edge
    //pt.EdgeTess[1] = 2; // Right edge
    //pt.EdgeTess[2] = 3; // Bottom edge
    //pt.InsideTess[0] = 4; // inside

    //pt.EdgeTess[0] = 6; // Left edge
    //pt.EdgeTess[1] = 6; // Right edge
    //pt.EdgeTess[2] = 6; // Bottom edge
    //pt.InsideTess[0] = 3; // inside

    //pt.EdgeTess[0] = 6; // Left edge
    //pt.EdgeTess[1] = 12; // Right edge
    //pt.EdgeTess[2] = 3; // Bottom edge
    //pt.InsideTess[0] = 1; // inside

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
             float3 uvw : SV_DomainLocation, //For a tri patch, use a float3 (for barycentric coordinates)
             const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout; 

    float3 p = tri[0].PosL * uvw.x + tri[1].PosL * uvw.y + tri[2].PosL * uvw.z;
	
	
	float4 posW = mul(float4(p, 1.0f), gWorld);
	dout.PosH = mul(posW, gViewProj);

    dout.Color = tri[0].Color;
	return dout;
}




