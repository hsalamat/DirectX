
//step5: note that there is no "main" any more..that means you need to change the entry point name to PS in HLSL compiler!

cbuffer cbPerFaceColor: register(b1)
{
	float4 face_colors[6];
};


float4 PS(uint tid : SV_PrimitiveID) : SV_Target
{

	//if (tid == 0 || tid == 1) return face_colors[0];
	//if (tid == 2 || tid == 3) return face_colors[1];
	//if (tid == 4 || tid == 5) return face_colors[2];
	//if (tid == 6 || tid == 7) return face_colors[3];
	//if (tid == 8 || tid == 9) return face_colors[4];
	//if (tid == 10 || tid == 11) return face_colors[5];
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return face_colors[tid/2]; //12 triangles - 6 faces

}