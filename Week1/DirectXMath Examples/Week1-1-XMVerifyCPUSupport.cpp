/** @file Week1-1-XMVerifyCPUSupport.cpp
 *  @brief XMVerifyCPUSupport Demo
 *
 *  DirectX Math is a 3D math library for Direct3D
 *  application that is part of the Windows SDK.The library uses the SSE2(Streaming SIMD
 *  Extensions 2) instruction set.With 128 - bit wide SIMD(single instruction multiple data)
 *  registers, SIMD instructions can operate on four 32 - bit floats or ints with one instruction.
 *
 *  @note use XMVerifyCPUSupport to check support for SSE2 (Pentium4, AMD K8, and above).
 *  @attention use #include <windows.h> for XMVerifyCPUSupport
 *
 *  In addition, for the x86 platform you should enable SSE2 (Project Properties > Configuration Properties >
 *  C/C++ > Code Generation > Enable Enhanced Instruction Set), and for all platforms
 *  you should enable the fast floating point model /fp:fast (Project Properties >
 *  Configuration Properties > C/C++ > Code Generation > Floating Point Model). You
 *  do not need to enable SSE2 for the x64 platform because all x64 CPUs support SSE2
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

int main()
{
	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if (!XMVerifyCPUSupport())
	{
		cout << "directx math not supported" << endl;
		return 0;
	}

	return 0;
}