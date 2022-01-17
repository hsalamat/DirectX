/** @file Week2-1-win32Demo.cpp
 *  @brief Illustrates the minimal amount of the Win32 code needed for Direct3D programming.
 *
 *  Windows is about two things: windows (register window class, create an instance) and messages. 
 *  Include the windows header file; this has all the Win32 API
 *  structures, types, and function declarations we need to program Windows.
 *
 *  @attention: Program requires Empty Project!
 *  @note: Go to Properties --> Linker --> System --> Change the subsystem to "Windows"
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */


#include <windows.h>

//! https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)

{
	//step1
	//while (true);


	//register window class
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	const auto pClassName = L"BasicWndClass";
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; //own its Device Context
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;   //!extra memory slots
	wc.cbWndExtra = 0;    //!extra memory slots
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr; //!spcifies the windows menu. We have no menu.
	//wc.lpszClassName = L"BasicWndClass";
	wc.lpszClassName = pClassName;
	RegisterClassEx(&wc);

	//create window
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	HWND hWnd = CreateWindowEx(
	0,
	pClassName,
    L"My First Window",
    WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
	200,200,640,480,
	nullptr, nullptr,hInstance,nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
	while (true);

	return 0;
}