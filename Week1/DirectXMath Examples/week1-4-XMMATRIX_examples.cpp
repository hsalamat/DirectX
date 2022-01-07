/** @file week1-4-XMMATRIX_examples.cpp
 *  @brief using Matrix operations Demo
 *
 *  XMMATRIX uses four XMVECTOR instances to use SIMD
 *  Just as we use XMFLOAT2 (2D), XMFLOAT3 (3D), and XMFLOAT4 (4D) when storing
 *  vectors in a class, it is recommended, by the DirectXMath documentation to use the
 *  XMFLOAT4X4 type to store matrices as class data members
 * 
 *  We use the following method to load data from XMFLOAT4X4 into XMMATRIX:
 *  inline XMMATRIX XM_CALLCONV XMLoadFloat4x4(const XMFLOAT4X4* pSource);
 * 
 *  We use the following method to store data from XMMATRIX into XMFLOAT4X4:
 *  inline void XM_CALLCONV XMStoreFloat4x4(XMFLOAT4X4* pDestination, FXMMATRIXM);
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

	XMMATRIX A(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 4.0f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f);

	XMMATRIX B = XMMatrixIdentity();

	XMMATRIX C = A * B;

	//or you can call xmmultiply
	XMMATRIX C1 = XMMatrixMultiply(A, B);

	XMMATRIX D = XMMatrixTranspose(A);

	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX E = XMMatrixInverse(&det, A);

	XMMATRIX F = A * E;

	cout << "A = " << endl << A << endl;
	cout << "B = " << endl << B << endl;
	cout << "C = A*B = " << endl << C << endl;
	cout << "C1 = XMMatrixMultiply(A,B) = " << endl << C1 << endl;
	cout << "D = transpose(A) = " << endl << D << endl;
	cout << "det = determinant(A) = " << det << endl << endl;
	cout << "E = inverse(A) = " << endl << E << endl;
	cout << "F = A*E = " << endl << F << endl;

	return 0;
}