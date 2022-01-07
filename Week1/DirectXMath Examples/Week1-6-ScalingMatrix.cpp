/** @file Week1-6-ScalingMatrix.cpp
 *  @brief Scaling Demo using XMVector3Transform or XMMatrixScaling or XMMatrixScalingFromVector
 *  
 *  Constructs a scaling matrix:
 *  XMMATRIX XM_CALLCONV XMMatrixScaling(
 *  float ScaleX,
 *  float ScaleY,
 *  float ScaleZ); // Scaling factors
 * 
 * 
 *  Constructs a scaling matrix from components in vector:
 *  XMMATRIX XM_CALLCONV XMMatrixScalingFromVector( FXMVECTOR Scale); // Scaling factors (sx, sy,sz)
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */

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

	XMMATRIX S(0.5f, 0.0f, 0.0f, 0.0f,
			   0.0f, 2.0f, 0.0f, 0.0f,
			   0.0f, 0.0f, 1.0f, 0.0f,
			   0.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX S2 = XMMatrixScaling(0.5f, 2.0f, 1.0f);

	XMVECTOR Sc = XMVectorSet(0.5f, 2.0f, 1.0f, 1.0f);

	XMMATRIX S3 = XMMatrixScalingFromVector(Sc);

	// @note S1, S2, and S3 are all equal

	XMVECTOR minPoint = XMVectorSet(-4.0f, -4.0f, 0.0f, 0.0f);
	XMVECTOR maxPoint = XMVectorSet(4.0f, 4.0f, 0.0f, 0.0f);

	XMVECTOR scaledMinPoint = XMVector4Transform(minPoint , S);
	XMVECTOR scaledMaxPoint = XMVector4Transform(maxPoint , S);

	cout << "scaledMinPoint = minPoint x S = " << endl << scaledMinPoint << endl;
	cout << "scaledMinPoint = maxPoint x S = " << endl << scaledMaxPoint << endl;

	XMVECTOR scaledMinPoint3D = XMVector3Transform(minPoint, S);
	XMVECTOR scaledMaxPoint3D = XMVector3Transform(maxPoint, S);

	cout << "scaledMinPoint3D = minPoint x S = " << endl << scaledMinPoint << endl;
	cout << "scaledMinPoint3D = maxPoint x S = " << endl << scaledMaxPoint << endl;

	return 0;
}