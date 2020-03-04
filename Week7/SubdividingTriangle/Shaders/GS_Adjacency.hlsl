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



[maxvertexcount(12)]
void GS(triangleadj VertexOut gin[6], //PrimitiveType InputVertexType InputName[NumElements]-> one vertex for a point, two for aline, three for atriangle, four for line with adjacency, and six for a triangle with adjacency.
        //uint primID : SV_PrimitiveID, //inout StreamOutputObject<OutputVertexType>
        inout TriangleStream<GeoOut> triStream)  //OutputName
{

    GeoOut gout[6];
    [unroll]
    for (uint i = 0; i < 6; i=i+2)    //0,2,4 triangle
    {
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(gin[i].PosW, 1.0f), gWorldViewProj);
        gout[i].Color = gin[i].Color;
        triStream.Append(gout[i]);
    }

    //Remember For line and triangle(triStream) GS always outputs a line strip or a triangle strip! If you wanted to output triangle list, you need to call RestartStrip() every time three vertices appended to output stream.
    triStream.RestartStrip();
    [unroll]
    for ( i = 0; i < 3; i ++) //0,1,2 triangle
    {
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(gin[i].PosW, 1.0f), gWorldViewProj);
        gout[i].Color = gin[i].Color;
        triStream.Append(gout[i]);
    }
    


    triStream.RestartStrip();
    [unroll]
    for ( i = 2; i < 5; i++) //2,3,4 triangle
    {
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(gin[i].PosW, 1.0f), gWorldViewProj);
        gout[i].Color = gin[i].Color;
        triStream.Append(gout[i]);
    }
    
    triStream.RestartStrip();

    //0,4,5 triangle

    gout[0].PosH = mul(float4(gin[0].PosW, 1.0f), gWorldViewProj);
    gout[0].Color = gin[0].Color;
    triStream.Append(gout[0]);

    [unroll]
    for ( i = 4; i < 6; i++)
    {
        gout[i].PosH = mul(float4(gin[i].PosW, 1.0f), gWorldViewProj);
        gout[i].Color = gin[i].Color;
        triStream.Append(gout[i]);
    }


}
