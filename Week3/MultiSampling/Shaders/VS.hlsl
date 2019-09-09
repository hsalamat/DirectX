float4 main( float3 pos : POSITION ) : SV_POSITION   // special semantic: System Value Position
{
		return float4(pos.x, pos.y, pos.z, 1.0f);
}