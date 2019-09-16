//why zero parameter? the rasterizer determines the pixel positions; 
//the pixel shader just determines the colors of each of those pixels.
//It usually doesn't need the coordinate of a pixel in screen space in order to accomplish this task.

float4 main() : SV_TARGET  //stands for render target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}