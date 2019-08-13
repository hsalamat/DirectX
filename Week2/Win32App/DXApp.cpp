#include "DXApp.h"


namespace {
	//used to forward messages to user defined proc function
	DXApp* g_App = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_App) g_App->MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance;
	m_hAppWnd = NULL; //we don't have a window yet
	m_ClientWidth = 800;
	m_ClientHeight = 800;
	m_AppTitle = "DirectX12 Application";
	m_WndStyle = WS_OVERLAPPEDWINDOW; //basic maximize, minimize, etc.. 
}

DXApp::~DXApp()
{
}

int DXApp::Run()
{
	//Main Message Loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) //peek and remove from the stack
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			//In off-time we update and render
			Update(0.0);
			Render(0.0);
		}
	}

	return static_cast<int>(msg.wParam);
}

bool DXApp::Init()
{
	if(!InitWindow())
	return false;
	return true;
}

bool DXApp::InitWindow()
{
	//WNDCLASSEX - Bear in mind that the only difference between CreateWindow and CreateWindowEx is the addition of extended styles to the latter. You can use either type of window class structure with them.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX)); //Fills a block of memory with zeros.
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = m_hAppInstance;
	wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"DXAPPWNDCLASS";

	//register with Windows OS
	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString(L"Failed to create Window Class\n");
		return false;
	}

	RECT r = { 0,0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CXSCREEN) / 2 - height / 2;
	m_hAppWnd = CreateWindow(L"DXAPPWNDCLASS", L"test", m_WndStyle, x, y, width, height, NULL, NULL, m_hAppInstance, NULL);

	if (!m_hAppWnd) {
		OutputDebugString(L"Failed to create Window \n");
		return false;
	}

	ShowWindow(m_hAppWnd, SW_SHOW);

	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	PostQuitMessage(0);
	return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); //wParam, lParam..mouse position..ket down...
	}
	
}
