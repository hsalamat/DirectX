//step14
cbuffer cbPerObject : register(b0)
{
	//float4x4 or matrix are the same;;
	//float4x4 gWorldViewProj;
	matrix gWorldViewProj;
};
//

struct VSOut
{
	float4 color : COLOR;  //notice the color has to be before position to match the color in pixel shader input parameter!
	float4 pos: SV_POSITION; // special semantic: System Value Position

};
VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut  vso;
	vso.pos = float4(pos.x, pos.y, pos.z, 1.0f);

	//step15
	// Transform to homogeneous clip space.
	//matrix multiplication is "right multiply", vector has to be on the left side, matrix on the right side
	vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), gWorldViewProj);


	vso.color = color; //we pass the color AS IS!
	return vso;
}