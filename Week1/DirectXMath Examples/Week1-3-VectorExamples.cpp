/** @file Week1-3-VectorExamples.cpp
 *  @brief using vector operations Demo
 *
 *  1. Use XMVECTOR for local or global variables.XMVECTOR needs to be 16-byte aligned. This is automatically done for local and global variables.
 *  2. Use XMFLOAT2, XMFLOAT3, and XMFLOAT4 for class data members.
 *  3. Use loading functions to convert from XMFLOATn to XMVECTOR before doing calculations.
 *  4. Do calculations with XMVECTOR instances.
 *  5. Use storage functions to convert from XMVECTOR to XMFLOATn.
 *  6. The calling convention annotation XM_CALLCONV must be
 *     specified before the function name so that the proper calling convention is used
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
// output XMVECTOR objects.
ostream& XM_CALLCONV operator << (ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

int main()
{
	//When the boolalpha format flag is set, bool values are inserted / extracted by their textual representation : either true or false, instead of integral values
	cout.setf(ios_base::boolalpha);

	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

	// Vector addition: XMVECTOR operator + 
	XMVECTOR a = u + v;

	// Vector subtraction: XMVECTOR operator - 
	XMVECTOR b = u - v;

	// Scalar multiplication: XMVECTOR operator * 
	XMVECTOR c = 10.0f * u;

	// ||u||
	XMVECTOR L = XMVector3Length(u);

	// d = u / ||u||
	XMVECTOR d = XMVector3Normalize(u);

	// s = u dot v
	XMVECTOR s = XMVector3Dot(u, v);

	// e = u x v
	XMVECTOR e = XMVector3Cross(u, v);

	// Find proj_n(w) and perp_n(w)
	// If we interpret w as a force, projW can be thought of as the portion of the force w that acts in
	// the direction n.Likewise, the vector perpW = perpn(w) = w – p is the portion of the force v that
	// acts orthogonal to the direction n
	XMVECTOR projW;  //projW the orthogonal projection of w on n,
	XMVECTOR perpW;
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);

	// Does projW + perpW == w?
	bool equal = XMVector3Equal(projW + perpW, w) != 0;
	bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;

	// The angle between projW and perpW should be 90 degrees.
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);

	cout << "u                   = " << u << endl;
	cout << "v                   = " << v << endl;
	cout << "w                   = " << w << endl;
	cout << "n                   = " << n << endl;
	cout << "a = u + v           = " << a << endl;
	cout << "b = u - v           = " << b << endl;
	cout << "c = 10 * u          = " << c << endl;
	cout << "d = u / ||u||       = " << d << endl;
	cout << "e = u x v           = " << e << endl;
	cout << "L  = ||u||          = " << L << endl;
	cout << "s = u.v             = " << s << endl;
	cout << "projW               = " << projW << endl;
	cout << "perpW               = " << perpW << endl;
	cout << "projW + perpW == w  = " << equal << endl;
	cout << "projW + perpW != w  = " << notEqual << endl;
	cout << "angle               = " << angleDegrees << endl;

	return 0;
}


