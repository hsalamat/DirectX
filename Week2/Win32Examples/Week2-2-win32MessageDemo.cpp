/** @file Week2-2-win32MessageDemo.cpp
 *  @brief Illustrates the minimal amount of the Win32 code needed for Direct3D programming.
 *
 *  Windows is about two things: windows (register window class, create an instance) and messages.
 *  @note: Even when we close the window, the program is still running
 *  Because DefWindowProc has no idea if you are closign a window or closing the whole application
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
	WNDCLASSEX wc = { 0 };
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
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);
	//while (true);
	MSG msg = { 0 };
	//! Loop until we get a WM_QUIT message. The function
	//! GetMessage will only return 0 (false) when a WM_QUIT message
	//! is received, which effectively exits the loop. The function
	//! returns -1 if there is an error. Also, note that GetMessage
	//! puts the application thread to sleep until there is a
	//! message.
	//! https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-quit
	BOOL bRet = 1;
	while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (bRet == -1)
	{
		MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
		return bRet;
	}
	else
	{
		return 0;
	}
}