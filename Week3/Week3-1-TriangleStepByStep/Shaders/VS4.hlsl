//step4: note that there is no "main" any more..that means you need to change the entry point name to VS in HLSL compiler!
cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};



float4 VS(float3 pos : POSITION) : SV_POSITION
{


	// Transform to homogeneous clip space.
	return mul(float4(pos.x , pos.y, pos.z, 1.0f), gWorldViewProj);


}