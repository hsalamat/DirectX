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

	XMVECTOR p = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR v = XMVectorSet(3.0f, 5.0f, 2.0f, 1.0f);
	float x = XMVectorGetX(XMPlaneDotCoord(p, v));
	if (x <= 0.001f && x >= 0.001f) // if x approximately equals 0.0f
		cout << " v is coplanar to the plane." << endl;
	if (x > 0)
		cout << "v is in positive half-space." << endl;
	if (x < 0) 
		cout << "v is in negative half-space." << endl;

	//This returns the dot product of the plane normal vector and the given 3D vector which is (5.0f, 5.0f, 5.0f, 5.0f), 
	//it means the shortest distance to the plane equals to 5. 
	XMVECTOR d =  XMPlaneDotNormal(p, v);
	cout << "The shortest distance to the plane: " << d << endl;

	//It is useful to think of plane as a 4D vector, which we denote as (n, d) = (a, b, c, d). 
	//𝑛.(𝑝−𝑝0) = 𝑛.𝑝 – 𝑛𝑝0 = 𝑛.𝑝 + 𝑑 =0  where d = −n · p0
	//to construct a plane from a point and normal, you could use:
	//XMVECTOR XMPlaneFromPointNormal( XMVECTOR Point, XMVECTOR Normal);
	cout << "Construct a plane vector (n,d) from Point-Normal: " << XMPlaneFromPointNormal(v, p) << endl; //(0.0f, 1.0f, 0.0f, -5.0f)

	//compute a plane given three points on the plane :
    //XMVECTOR XMPlaneFromPoints(XMVECTOR Point1, XMVECTOR Point2, XMVECTOR Point3);


	XMVECTOR p0 = XMVectorSet(0.5f, 1.0f, 0.0f, 1.0f);
	XMVECTOR p1 = XMVectorSet(0.0f, 1.0f, 0.5f, 1.0f);
	XMVECTOR p2 = XMVectorSet(-0.5f, 1.0f, 0.0f, 1.0f);
	cout << "Construct a plane vector (n,d) from three points: " << XMPlaneFromPoints(p0, p1, p2) << endl; //(0, -1, 0, 1)


	//Sometimes we might have a planeand would like to normalize the normal vector.At first thought, 
	//it would seem that we could just normalize the normal vector as we would any other vector.
	//But recall that the d component also depends on the normal vector : d = −n · p0.Therefore, 
	//if we normalize the normal vector, we must also recalculate d.
	//We can use the following DirectX Math function to normalize a plane’s normal vector :
	//XMVECTOR XMPlaneNormalize(XMVECTOR P);
	cout << "To normalize a plane’s normal vector :" << XMPlaneNormalize(p) << endl; //(0, -1, 0, 1)

	//The set of intersection points found by shooting a ray through each of the object’s vertices with the plane
	//defines the projected geometry of the shadow. For a vertex p, its shadow projection is given by:
	//XMVECTOR XMPlaneIntersectLine(
	//	XMVECTOR P, => the plane coefficients (A, B, C, D)
	//	XMVECTOR LinePoint1, => the first point on the line
	//	XMVECTOR LinePoint2); => the second point on the line

	XMVECTOR LinePoint1 = XMVectorSet(10.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR LinePoint2 = XMVectorSet(0.0f, -5.0f, 0.0f, 1.0f);
	cout << "The intersection of the ray r(t) with the shadow plane (n, d) gives s: "
		<< XMPlaneIntersectLine(XMPlaneFromPointNormal(v, p), LinePoint1, LinePoint2) << endl;

	//Builds a transformation matrix that flattens geometry into a plane.
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR lightPosition = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);
	XMMATRIX S = XMMatrixShadow(shadowPlane, lightPosition);
	cout << "The transformation matrix that flattens geometry into a plane given a light position: " << endl << S << endl;

	system("pause");
	return 0;
}

