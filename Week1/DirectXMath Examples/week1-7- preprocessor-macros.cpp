
//The Microsoft C/C++ compiler (MSVC) predefines certain preprocessor macros, depending on the language (C or C++), the compilation target, and the chosen compiler options.
//change x86 to x64 to see the difference

//https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019

#include <stdio.h>
#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
int main() {

#if (defined(_M_IX86)) //Defined as the integer literal value 600 for compilations that target x86 processors.
	cout << _M_IX86 << endl;;
#endif

#if (defined(_M_X64)) // Defined as the integer literal value 100 for compilations that target x64 processors.
	cout << _M_X64 << endl;;
#endif

#if (defined(_M_ARM_))  //Defined as the integer literal value 7 for compilations that target ARM processors. Otherwise, undefined.
	cout << _M_ARM_ << endl;;
#endif


#define _XM_NO_INTRINSICS_
	//When _XM_NO_INTRINSICS_ is defined, DirectXMath operations are implemented without using any platform - specific intrinsics.Instead, DirectXMath uses only standard floating point operations.
#if (defined(_XM_NO_INTRINSICS_))  //Defined as the integer literal value 7 for compilations that target ARM processors. Otherwise, undefined.
	cout << "_XM_NO_INTRINSICS_ on --> standard floating point operation" << endl;
#endif

//__declspec The extended attribute syntax for specifying storage-class information uses the __declspec keyword, 
//which specifies that an instance of a given type is to be stored with a Microsoft-specific storage-class attribute

//Use __declspec(align(#)) to precisely control the alignment of user-defined data 
	__declspec(align(32)) struct Str1 {
		int a, b, c, d, e;
	};

	//This means that all static and automatic instances start on a 32-byte boundary.

	cout << sizeof(struct Str1) << endl;

	Str1 str1 = { 1,2,3,4,5 };
	
	cout << str1.a << endl;
	cout << sizeof(str1.a) << endl;

	return 0;

}
