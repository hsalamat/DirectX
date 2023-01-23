/** @file memcpydemo.cpp
 *  @brief Copy block of memory
 *   Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.
 *
 *   The underlying type of the objects pointed to by both the source and destination pointers are irrelevant for this function; 
 *   The result is a binary copy of the data.
 *
 *   The function does not check for any terminating null character in source - it always copies exactly num bytes.
 *
 *  @author Hooman Salamat
 */
#include <iostream>
#include <windows.h>

using namespace std;

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
} Vertex0, Vertex1, Vertex2, Vertex3;

int main()
{
	Vertex0 = { XMFLOAT3(-0.5f, -0.5f, 0.0f) };
	Vertex2 = { XMFLOAT3(-0.5f, -0.5f, 0.0f) };

	/* using memcpy to copy vertices: */
	memcpy(&Vertex1, &Vertex0, sizeof(Vertex));


	cout << "Vertex1: " << Vertex1.Pos.x << "," << Vertex1.Pos.y<<","<< Vertex1.Pos.z << endl;

	//can you believe this? there is also something CopyMemory in windows.h does the samething :-)

	CopyMemory(&Vertex3, &Vertex2, sizeof(Vertex));

	cout << "Vertex3: " << Vertex3.Pos.x << "," << Vertex3.Pos.y << "," << Vertex3.Pos.z << endl;

	return 0;
}