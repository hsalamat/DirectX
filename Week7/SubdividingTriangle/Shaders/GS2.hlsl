cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};



struct VertexOut
{
    float3 CenterW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
};

struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 Color : COLOR;
    uint PrimID : SV_PrimitiveID;
};




//step4
 // We expand each point into a quad (4 vertices), so the maximum number of vertices
 // we output per geometry shader invocation is 4.
[maxvertexcount(4)]
void GS(point VertexOut gin[1], //PrimitiveType InputVertexType InputName[NumElements]-> one vertex for a point, two for aline, threefor atriangle, four for line with adjacency, and six for a triangle with adjacency.
        uint primID : SV_PrimitiveID, //inout StreamOutputObject<OutputVertexType>
        inout TriangleStream<GeoOut> triStream)  //OutputName
{

    	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	//

    float3 up = float3(0.0f, 1.0f, 0.0f);
    //float3 look = gEyePosW - gin[0].CenterW;
    float3 look = float3(0.0f, 0.0f, -1.0f);
    look.y = 0.0f; // y-axis aligned, so project to xz-plane
    look = normalize(look);
    float3 right = cross(up, look);
//
	// Compute triangle strip vertices (quad) in world space.
	//
    float halfWidth = 0.5f * gin[0].SizeW.x;
    float halfHeight = 0.5f * gin[0].SizeW.y;
	
    float4 v[4];
    v[0] = float4(gin[0].CenterW + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(gin[0].CenterW + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(gin[0].CenterW - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(gin[0].CenterW - halfWidth * right + halfHeight * up, 1.0f);


    GeoOut gout;
	[unroll]
    for (int i = 0; i < 4; ++i)
    {

        gout.PosH = mul(v[i], gWorldViewProj);
        gout.PosW = v[i].xyz;
        gout.Color = gin[0].Color;
        gout.PrimID = primID;
		
        triStream.Append(gout);
    }
}
