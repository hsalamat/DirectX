/** @file Week1-2-ConstantXMVECTOR.cpp
 *  @brief using constant XMVector Demo
 *
 *  In DirectX Math, the core vector type is XMVECTOR, which maps to SIMD hardware
 *  registers. This is a 128-bit type that can process four 32-bit floats with a single SIMD
 *  instruction. When SSE2 is available, it is defined like so for x86 and x64 platforms:
 *  typedef __m128 XMVECTOR;
 *  Constant XMVECTOR instances should use the XMVECTORF32 type.
 *  The XMVECTORF32, XMVECTORU32, XMVECTORI32, and XMVECTORU8 structures are provided as a mechanism
 *	for creating XMVECTOR from different constant data types (floating point, unsigned integer, integer, and byte) using initializers.
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

	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	//Constant XMVECTOR instances should use the XMVECTORF32 type.
	static const XMVECTORF32 g_vHalfVector = { 0.5f, 0.5f, 0.5f, 0.5f };
	static const XMVECTORF32 g_vZero = { 0.0f, 0.0f, 0.0f, 0.0f };

	//The XMVECTORF32, XMVECTORU32, XMVECTORI32, and XMVECTORU8 structures are provided as a mechanism 
	//for creating XMVECTOR from different constant data types (floating point, unsigned integer, integer, and byte) using initializers.
	static const XMVECTORU32 vGrabY = { 0x00000000,0xEFFFFFFF,0x00000000,0x00000000 };
	XMVECTOR data;
	data = vGrabY;
	cout << "data = " << data << endl;

	// Returns the zero vector 0
	XMVECTOR p = XMVectorZero();

	// Returns the vector (1, 1, 1, 1)
	XMVECTOR q = XMVectorSplatOne();

	// Returns the vector (x, y, z, w)
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);

	// Returns the vector (s, s, s, s)
	XMVECTOR v = XMVectorReplicate(-2.0f);

	// Returns the vector (wz, wz, wz, wz)
	XMVECTOR w = XMVectorSplatZ(u);

	cout << "p = " << p << endl;
	cout << "q = " << q << endl;
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "w = " << w << endl;

	const double d = 1.2345678901234;
	std::cout << "The  default precision is " << std::cout.precision() << endl;
	std::cout << "With default precision d is " << d << endl;
	std::cout.precision(12);
	std::cout << "With high    precision d is " << d << endl;


	//Manages the precision (i.e. how many digits are generated) of floating point output
	cout.precision(8);

	XMVECTOR n = XMVector3Normalize(u);

	float LU = XMVectorGetX(XMVector3Length(n));

	// Mathematically, the length should be 1. Is it numerically ?
	cout << "LU = " << LU << endl;
	if (LU == 1.0f)
		cout << "Length 1" << endl;
	else
		cout << "Length not 1" << endl;

	// Raising 1 to any power should still be 1. Is it?
	float powLU = powf(LU, 1.0e6f);
	cout << "LU ^ (10 ^ 6) = " << powLU << endl;

	//To compensate for floating-point imprecision, we test if two floating-point numbers
	//are approximately equal.We do this by defining an Epsilon constant, which is a very small
	//value we use as a “buffer.”

	const float Epsilon = 0.001f;
	//bool Equals(float lhs, float rhs)
	//{
	//	// Is the distance between lhs and rhs less than EPSILON ?
	//		return fabs(lhs - rhs) < Epsilon ? true : false;
	//}

	//The DirectX Math library provides the XMVector3NearEqual function when
	//	testing the equality of vectors with an allowed tolerance Epsilon parameter :
	// Returns
	// abs(U.x – V.x) <= Epsilon.x &&
	// abs(U.y – V.y) <= Epsilon.y &&
	// abs(U.z – V.z) <= Epsilon.z
	//XMFINLINE bool XM_CALLCONV XMVector3NearEqual(	FXMVECTOR U,FXMVECTOR V, FXMVECTOR Epsilon);

	//When the boolalpha format flag is set, bool values are inserted / extracted by their textual representation : either true or false, instead of integral values
	cout.setf(ios_base::boolalpha);
	cout << "LU and q vectors are almost equal? " << XMVector3NearEqual(XMVectorReplicate(LU), q, XMVectorReplicate(Epsilon)) << endl;


	p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
	q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
	u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
	v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
	w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);

	cout << "XMVectorAbs(v) = " << XMVectorAbs(v) << endl;
	cout << "XMVectorCos(w) = " << XMVectorCos(w) << endl;
	cout << "XMVectorLog(u) = " << XMVectorLog(u) << endl;
	cout << "XMVectorExp(p) = " << XMVectorExp(p) << endl;
	cout << "XMVectorPow(u, p) = " << XMVectorPow(u, p) << endl;
	cout << "XMVectorSqrt(u) = " << XMVectorSqrt(u) << endl;
	cout << "XMVectorSwizzle(u, 2, 2, 1, 3) = " << XMVectorSwizzle(u, 2, 2, 1, 3) << endl;
	cout << "XMVectorSwizzle(u, 2, 1, 0, 3) = " << XMVectorSwizzle(u, 2, 1, 0, 3) << endl;
	cout << "XMVectorMultiply(u, v) = " << XMVectorMultiply(u, v) << endl;
	cout << "XMVectorSaturate(q) = " << XMVectorSaturate(q) << endl;
	cout << "XMVectorMin(p, v) = " << XMVectorMin(p, v) << endl;
	cout << "XMVectorMax(p, v) = " << XMVectorMax(p, v) << endl;

	return 0;
}