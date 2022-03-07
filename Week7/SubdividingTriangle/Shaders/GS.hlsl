cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};



struct VertexOut
{
    float3 PosW : POSITION;
    float4 Color : COLOR;
};

struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float4 Color : COLOR;
    uint PrimID : SV_PrimitiveID;
};


void Subdivide(VertexOut inVerts[3], out VertexOut outVerts[6])
{
    VertexOut m[3];
    // Compute edge midpoints.
    m[0].PosW = 0.5f * (inVerts[0].PosW + inVerts[1].PosW);
    m[1].PosW = 0.5f * (inVerts[1].PosW + inVerts[2].PosW);
    m[2].PosW = 0.5f * (inVerts[2].PosW + inVerts[0].PosW);

    // Interpolate color coordinates.
    m[0].Color = 0.5f * (inVerts[0].Color + inVerts[1].Color);
    m[1].Color = 0.5f * (inVerts[1].Color + inVerts[2].Color);
    m[2].Color = 0.5f * (inVerts[2].Color + inVerts[0].Color);


    outVerts[0] = inVerts[0];
    outVerts[1] = m[0];
    outVerts[2] = m[2];
    outVerts[3] = m[1];
    outVerts[4] = inVerts[2];
    outVerts[5] = inVerts[1];
};

void OutputSubdivision(VertexOut v[6], inout TriangleStream<GeoOut> triStream, uint primID)
{
    GeoOut gout[6];
    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(v[i].PosW, 1.0f), gWorldViewProj);
        gout[i].Color = v[i].Color;
        gout[i].PrimID = primID;
    }
    

    [unroll]
    for (int j = 0; j < 5; ++j) //  - > p0, m0, m2, m1, p2
    {
        triStream.Append(gout[j]); // Strip 1: bottom three triangles (5 vertices)
    }

    //The vertices output by a geometry shader form primitives; the type of output primitive
    //is indicated by the stream type (PointStream, LineStream, TriangleStream).
    //For lines and triangles, the output primitive is always a strip. Line and triangle lists,
    //however, can be simulated by using the intrinsic RestartStrip method:

    triStream.RestartStrip(); //Strip 2: top triangle (three vertices)

    triStream.Append(gout[1]); //m0

    triStream.Append(gout[5]); //p1

    triStream.Append(gout[3]); //m1


}

[maxvertexcount(8)]
void GS(triangle VertexOut gin[3], //PrimitiveType InputVertexType InputName[NumElements]-> one vertex for a point, two for aline, threefor atriangle, four for line with adjacency, and six for a triangle with adjacency.
        uint primID : SV_PrimitiveID, //inout StreamOutputObject<OutputVertexType>
        inout TriangleStream<GeoOut> triStream)  //OutputName
{
    VertexOut v[6];
    Subdivide(gin, v);
    OutputSubdivision(v, triStream, primID);
}
