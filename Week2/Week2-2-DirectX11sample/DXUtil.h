#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#pragma comment (lib, "d3d11.lib")

namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
	if(t)
	{
		delete t;
		t=nullptr;
	}
	}
	
	template <class T> void SafeDeleteArr(T& t)
	{
	if(t)
	{
		delete[] t;
		t=nullptr;
	}
	}
	
	template <class T> void SafeRelease(T& t)
	{
	if(t)
	{
		t->Release();
		t=nullptr;
	}
	}
}
	
	