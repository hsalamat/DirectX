#pragma once
#define WIN32_LEAN_AND_MEAN //not cryptography and networking...
#include <Windows.h>
#include <string>
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

protected:
	bool InitWindow();

};

