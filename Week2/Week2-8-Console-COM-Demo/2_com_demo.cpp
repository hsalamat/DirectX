/** @file com-demo.cpp
 *  @brief COM demo.
 *
 *  The following code shows how to find an image file that is used as your desktop wall paper 
 *  and save it as a link on your desktop
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */


#include <Windows.h>
#include <Wininet.h>
#include <ShlObj_core.h>
#include <iostream>
using namespace std;
int main()
{
	//! Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
	HRESULT hr;
	/*hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		cout << "Failed to initialize COM library. Error code = 0x"
			<< hex << hr << endl;
		return hr;
	}*/
	//this is the simplified version of above...
	CoInitialize(nullptr);

	IActiveDesktop* pDesktop = nullptr;
	WCHAR wszWallpaper[MAX_PATH];
	//! factory function to create a COM object: a factory is a function that returns objects of a class from some method call
	hr = CoCreateInstance(
		CLSID_ActiveDesktop,  //underlying COM object that we want to create
		nullptr,
		CLSCTX_INPROC_SERVER,   //context of the object --> process context
		__uuidof(IActiveDesktop),
		reinterpret_cast<void**>(&pDesktop)  //this pointer will be filled
	);
	pDesktop->GetWallpaper(wszWallpaper, MAX_PATH, 0);
	pDesktop->Release();
	std::wcout << wszWallpaper;


#pragma region Add this section below CoUninitialize
	//! the following section create a short cut to the wall paper
	IShellLink* pLink = nullptr;
	hr = CoCreateInstance(
		CLSID_ShellLink,  //underlying COM object that we want to create
		nullptr,
		CLSCTX_INPROC_SERVER,   //context of the object --> process context
		__uuidof(IShellLink),
		reinterpret_cast<void**>(&pLink)  //this pointer will be filled
	);
	pLink->SetPath(wszWallpaper);
	IPersistFile* pPersist = nullptr;
	pLink->QueryInterface(__uuidof(IPersistFile), reinterpret_cast<void**> (&pPersist));
	//! change this to a "proper path"
	pPersist->Save(L"wallpaper.lnk", FALSE);
	pPersist->Release();
	pLink->Release();
#pragma endregion

	CoUninitialize();

	std::cin.get();
	return 0;
}

