
//step5: note that there is no "main" any more..that means you need to change the entry point name to PS in HLSL compiler!

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}