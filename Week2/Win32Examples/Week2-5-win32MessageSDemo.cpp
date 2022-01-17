/** @file Week2-4-win32MessageSDemo
 *  @brief Illustrates the minimal amount of the Win32 code needed for Direct3D programming.
 *
 *  https://wiki.winehq.org/List_Of_Windows_Messages
 *  @note: Even when we close the window, the program is still running
 *  Now we are creating a custom window procedure to handle the closing the window better
 *  @attention: Program requires Empty Project!
 *  @note: Go to Properties --> Linker --> System --> Change the subsystem to "Windows"
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */


#include <windows.h>
#include <string>
#include <sstream> 
#include "WindowsMessageMap.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	std::string message = mm(msg, wParam, lParam);
	OutputDebugStringA(message.c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(100); //if you like to return 100
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"F has been pressed");
		}
		break;
	case WM_CHAR:		
		{
		static std::string title;
		title.push_back((char)wParam);
		SetWindowTextA(hWnd, title.c_str());
		}
		break;		
	case WM_LBUTTONDOWN:
		{
		const POINTS pt MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowTextA(hWnd, oss.str().c_str());
		}
	}
	return DefWindowProcW(hWnd, msg, wParam, lParam);
}


//! https://docs.microsoft.com/en-us/windows/win32/learnwin32/winmain--the-application-entry-point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)

{
	//register window class
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	const auto pClassName = L"BasicWndClass";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; //own its Device Context
	wc.lpfnWndProc = WndProc;
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

		return msg.wParam;
	}
}