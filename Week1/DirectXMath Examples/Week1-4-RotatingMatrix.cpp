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


	XMVECTOR minPoint = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR maxPoint = XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
	XMMATRIX Ry = XMMatrixRotationY(-XM_PI / 6.0f);

	XMVECTOR rotatedMinPoint = XMVector4Transform(minPoint, Ry);
	XMVECTOR rotatedMaxPoint = XMVector4Transform(maxPoint, Ry);

	cout << "rotatedMinPoint = minPoint x Ry = " << endl << rotatedMinPoint << endl;
	cout << "rotatedMaxPoint = maxPoint x Ry = " << endl << rotatedMaxPoint << endl;

	return 0;
}

