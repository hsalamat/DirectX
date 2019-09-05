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
	g_App = this; //otherwise app doesn't stop when windows closes!
	
	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRenderTargetView = nullptr;
	m_pSwapChain = nullptr;
}

DXApp::~DXApp()
{
	//Cleanup Direct3D

	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
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
	if (!InitDirect3D())
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

bool DXApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG 
createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG 

D3D_DRIVER_TYPE driverTypes[] =
{
	D3D_DRIVER_TYPE_HARDWARE, //graphics card
	D3D_DRIVER_TYPE_WARP,     //if you don't have graphics card
	D3D_DRIVER_TYPE_REFERENCE 
};

UINT numDriverTypes = ARRAYSIZE(driverTypes);


D3D_FEATURE_LEVEL featureLevels[] = 
{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3
};
UINT numFeatureLevels = ARRAYSIZE(featureLevels);

DXGI_SWAP_CHAIN_DESC swapDesc;
ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
swapDesc.BufferCount = 1; //double buffer
swapDesc.BufferDesc.Width = m_ClientWidth;
swapDesc.BufferDesc.Height = m_ClientHeight;
swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
swapDesc.BufferDesc.RefreshRate.Numerator = 60;
swapDesc.BufferDesc.RefreshRate.Denominator = 1;
swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
swapDesc.OutputWindow = m_hAppWnd;
swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
swapDesc.Windowed = true;
swapDesc.SampleDesc.Count = 1;
swapDesc.SampleDesc.Quality = 0;
swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //alt_enter full screen

HRESULT result;
for(int i=0; i< numDriverTypes; i++)
{
	result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
	featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice,
	&m_FeatureLevel, &m_pImmediateContext);

	if (SUCCEEDED(result)) {
		m_DriverType = driverTypes[i];
		break;
	}
}

if (FAILED(result)) {
	OutputDebugString(L"Failed To Create Device and Swap chain");
	return false;
}

//CREATE RENDER TARGET VIEW
ID3D11Texture2D* m_pBackBufferTex = 0;
m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTex));
m_pDevice->CreateRenderTargetView(m_pBackBufferTex, nullptr, &m_pRenderTargetView);

//BIND RENDER TARGETVIEW
m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

//VIEWPORT CREATION
m_Viewport.Width = static_cast<float> (m_ClientWidth);
m_Viewport.Height = static_cast<float> (m_ClientHeight);

m_Viewport.TopLeftX = 0;
m_Viewport.TopLeftY = 0;
m_Viewport.MinDepth = 0.0f;
m_Viewport.MaxDepth = 1.0f;

//BIND Viewport
m_pImmediateContext->RSSetViewports(1, &m_Viewport);

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
