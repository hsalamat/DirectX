//step6: notice that we only have compute shader!

struct Data
{
	float3 v1;
	float2 v2;
};

//A structured buffer is simply a buffer of elements of the same type—essentially an array.
StructuredBuffer<Data> gInputA : register(t0);
StructuredBuffer<Data> gInputB : register(t1);
RWStructuredBuffer<Data> gOutput : register(u0);


[numthreads(32, 1, 1)]
void CS(int3 dtid : SV_DispatchThreadID
, int3 gtid : SV_GroupThreadID, int3 gid : SV_GroupID, int gi: SV_GroupIndex
)
{
	gOutput[dtid.x].v1 = gInputA[dtid.x].v1 + gInputB[dtid.x].v1;
	gOutput[dtid.x].v2 = gInputA[dtid.x].v2 + gInputB[dtid.x].v2;

    //same result
    //gOutput[gtid.x].v1 = gInputA[gtid.x].v1 + gInputB[gtid.x].v1;
    //gOutput[gtid.x].v2 = gInputA[gtid.x].v2 + gInputB[gtid.x].v2;

    //same result
    //gOutput[gi.x].v1 = gInputA[gi.x].v1 + gInputB[gi.x].v1;
    //gOutput[gi.x].v2 = gInputA[gi.x].v2 + gInputB[gi.x].v2;

    //same result
    //gOutput[gi].v1= gInputA[gi].v1 + gInputB[gi].v1;
    //gOutput[gi].v2 = gInputA[gi].v2 + gInputB[gi].v2;

    //this will create an error because we only have one group
    //gOutput[gid.x].v1 = gInputA[gid.x].v1 + gInputB[gid.x].v1;
    //gOutput[gid.x].v2 = gInputA[gid.x].v2 + gInputB[gid.x].v2;
}



