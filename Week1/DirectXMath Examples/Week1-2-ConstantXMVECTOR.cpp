#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

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

	//Constant XMVECTOR instances should use the XMVECTORF32 type.
	static const XMVECTORF32 g_vHalfVector = { 0.5f, 0.5f, 0.5f, 0.5f };
	static const XMVECTORF32 g_vZero = { 0.0f, 0.0f, 0.0f, 0.0f };

	//The XMVECTORF32, XMVECTORU32, XMVECTORI32, and XMVECTORU8 structures are provided as a mechanism 
	//for creating XMVECTOR from different constant data types (floating point, unsigned integer, integer, and byte) using initializers.
	static const XMVECTORU32 vGrabY = {0x00000000,0xFFFFFFFF,0x00000000,0x00000000};
	XMVECTOR data;
	data = vGrabY;

	return 0;
}