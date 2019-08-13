#pragma once
#define WIN32_LEAN_AND_MEAN //not cryptography and networking...
#include <Windows.h>
#include <string>
#include "DXUtil.h"
class DXApp
{
public: 
	DXApp(HINSTANCE hInstance); //instance of the application (not window!)
	virtual ~DXApp();

	//Main Application Loop
	int Run();

	//Framework Methods
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0; // dt for animation
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	//WIN32 Attributes
	HWND  m_hAppWnd;
	HINSTANCE m_hAppInstance;
	UINT m_ClientWidth;
	UINT m_ClientHeight;
	std::string m_AppTitle;
	DWORD m_WndStyle;
	
	//DirectX Attributes
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	IDXGISwapChain*  m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D_DRIVER_TYPE	m_DriverType;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D11_VIEWPORT m_Viewport;
	

protected:
	bool InitWindow();
	
	//Initialize Direct3D
	bool InitDirect3D();

};

