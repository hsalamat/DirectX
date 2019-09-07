
//step5
float4 main(float4 color : COLOR) : SV_TARGET  //stands for render target
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return color; //color attributed is interpolated by raterizer
}