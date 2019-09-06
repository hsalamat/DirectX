#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;


// Overload the  "<<" operators so that we can use cout to
// output XMVECTOR and XMMATRIX objects.


ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

ostream& XM_CALLCONV operator << (ostream& os, FXMMATRIX m)
{
	for (int i = 0; i < 4; ++i)
	{
		os << XMVectorGetX(m.r[i]) << "\t";
		os << XMVectorGetY(m.r[i]) << "\t";
		os << XMVectorGetZ(m.r[i]) << "\t";
		os << XMVectorGetW(m.r[i]);
		os << endl;
	}
	return os;
}



int main()
{
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	//make sure w = 1.0 for points!
	XMVECTOR minPoint = XMVectorSet(-8.0f, 2.0f, 0.0f, 1.0f);
	XMVECTOR maxPoint = XMVectorSet(-2.0f, 8.0f, 0.0f, 1.0f);
	
	//Suppose now that we wish to translate the square 12 units on the x-axis, −10.0 units on the y - axis,

	XMMATRIX T(1.0f, 0.0f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f, 0.0f,
			   0.0f, 0.0f, 1.0f, 0.0f,
			   12.0f, -10.0f, 0.0f, 1.0f);

	XMVECTOR translatedMinPoint = XMVector4Transform(minPoint, T);
	XMVECTOR translatedMaxPoint = XMVector4Transform(maxPoint, T);

	cout << "translatedMinPoint = minPoint x T = " << endl << translatedMinPoint << endl;
	cout << "translatedMaxPoint = maxPoint x T = " << endl << translatedMaxPoint << endl;

	//The inverse of the translation matrix is given by :
	XMMATRIX TI(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-12.0f, 10.0f, 0.0f, 1.0f);

	cout << "Let's apply the Inverse of Translation Matrix TI to new translated square: " << endl;

	XMVECTOR inversedMinPoint = XMVector4Transform(translatedMinPoint, TI);
	XMVECTOR inversedMaxPoint = XMVector4Transform(translatedMaxPoint, TI);

	cout << "inversedMinPoint = minPoint x TI = " << endl << inversedMinPoint << endl;
	cout << "inversedMaxPoint = maxPoint x TI = " << endl << inversedMaxPoint << endl;



	return 0;
}

