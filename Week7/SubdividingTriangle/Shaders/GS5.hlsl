#include "Default2.hlsl"

void Subdivide(VertexOut inVerts[3], out VertexOut outVerts[6])
{
    VertexOut m[3];
// Compute edge midpoints.
    m[0].PosL = 0.5f * (inVerts[0].PosL + inVerts[1].PosL);
    m[1].PosL = 0.5f * (inVerts[1].PosL + inVerts[2].PosL);
    m[2].PosL = 0.5f * (inVerts[2].PosL + inVerts[0].PosL);

    // Project onto unit sphere
    //m[0].PosL = normalize(m[0].PosL);
    //m[1].PosL = normalize(m[1].PosL);
    //m[2].PosL = normalize(m[2].PosL);

    //Derive normals
    m[0].NormalL = m[0].PosL;
    m[1].NormalL = m[1].PosL;
    m[2].NormalL = m[2].PosL;

    //Interpolate texture coordinates
    m[0].TexC = 0.5f * (inVerts[0].TexC + inVerts[1].TexC);
    m[1].TexC = 0.5f * (inVerts[1].TexC + inVerts[2].TexC);
    m[2].TexC = 0.5f * (inVerts[2].TexC + inVerts[0].TexC);


    outVerts[0] = inVerts[0];
    outVerts[1] = m[0];
    outVerts[2] = m[2];
    outVerts[3] = m[1];
    outVerts[4] = inVerts[2];
    outVerts[5] = inVerts[1];

};

void OutputSubdivision(VertexOut v[6], inout TriangleStream<GeoOut> triStream)
{
    GeoOut gout[6];
    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        // Transform to homogeneous clip space.
        gout[i].PosW = mul(float4(v[i].PosL, 1.0f), gWorld).xyz;
        gout[i].NormalW = mul(v[i].NormalL, (float3x3) gWorld);
        float4 posW = mul(float4(v[i].PosL, 1.0f), gWorld);
        gout[i].PosH = mul(posW, gViewProj);
        //gout[i].TexC = v[i].TexC;

        float4 texC = mul(float4(v[i].TexC, 0.0f, 1.0f), gTexTransform);
        gout[i].TexC = mul(texC, gMatTransform).xy;
    }
    

    [unroll]
    for (int j = 0; j < 5; ++j) //  - > p0, m0, m2, m1, p2
    {
        triStream.Append(gout[j]); // Strip 1: bottom three triangles (5 vertices)
    }

    triStream.RestartStrip(); //Strip 2: top triangle (three vertices)

    triStream.Append(gout[1]); //m0

    triStream.Append(gout[5]); //p1

    triStream.Append(gout[3]); //m1


}

[maxvertexcount(8)]
void GS(triangle VertexOut gin[3], inout TriangleStream<GeoOut> triStream) 
{
    VertexOut v[6];
    Subdivide(gin, v);
    OutputSubdivision(v, triStream);
}


//[maxvertexcount(32)]
//void GS(triangle VertexOut gin[3], inout TriangleStream<GeoOut> stream)
//{
//    float distance;
//    ComputeDistance(gin[0], distance);
//    [branch]
//    if (distance < 15.0f)
//    {
//        VertexOut v[6];
//        Subdivide(gin, v);
//        OutputSubdivision(v, stream);
//    }
//    else if (distance > 15.0f && distance < 30.0f)
//    {
//        // here i want to get double tessellation
//        VertexOut v[6];
//        Subdivide(gin, v);
//        OutputSubdivision(v, stream);
//    }
//    else if (distance > 30.0f)
//    {
//        GeoOut v[3];
//        for (int i = 0; i < 3; ++i)
//        {
//            v.PosW = mul(float4(gin.PosL, 1.0f), gWorld).xyz;
//            v.NormalW = mul(gin.NormalL, (float3x3)gWorldInvTranspose);
//            v.PosH = mul(float4(gin.PosL, 1.0f), gWorldViewProj);
//            v.Tex = mul(gin.Tex, (float3x3)gWorldInvTranspose);
//            stream.Append(v);
//        }
//    }
//}
