//step4
struct VSOut
{
	float4 color : COLOR;  //notice the color has to be before position to match the color in pixel shader input parameter!
	float4 pos: SV_POSITION; // special semantic: System Value Position

};
VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut  vso;
	vso.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	vso.color = color; //we pass the color AS IS!
	return vso;
}