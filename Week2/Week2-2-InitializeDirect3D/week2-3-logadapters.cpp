
#include "../../Common/d3dApp.h"
#include <DirectXColors.h>
#include <stdlib.h>
#include <string>
#include <comdef.h>

using namespace DirectX;
using namespace std;

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

};


//WinMain --> every win32 application needs a WinMain() function, like console application needs main()
//HINSTANCE, PSTR.. are part of <Windows.h>
//Windows is about Windows (window class that you register and instance of the class) and Messages
//cmdLine receives the parameter that somebody passes


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		InitDirect3DApp theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{

}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;


	//We can query the number of quality levels for a given texture format and sample count using the ID3D12Device::CheckFeatureSupport method like so :
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	//Note that the second parameter is both an input and output parameter. For the input,
	//we must specify the texture format, sample count, and flag we want to query
	//multisampling support for.The function will then fill out the quality level as the output.
	//Valid quality levels for a texture format and sample count combination range from zero to NumQualityLevels–1.
	//The maximum number of samples that can be taken per pixel is defined by : #define D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT ( 32 )

	ThrowIfFailed(md3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

	//Note that the second parameter is both an input and output parameter. For the input,
	//we specify the number of elements(NumFeatureLevels) in a feature level array, and
	//	a pointer to a feature level array(pFeatureLevelsRequested) which contains a list
	//	of feature levels we want to check hardware support for.The function outputs the
	//	maximum supported feature level through the MaxSupportedFeatureLevel field.

	//step1 what is the maximum feature that hardware support
	D3D_FEATURE_LEVEL featureLevels[5] =
	{
	D3D_FEATURE_LEVEL_12_1, // First check D3D 12.1 support,
	D3D_FEATURE_LEVEL_12_0, // Next check D3D 12 support
	D3D_FEATURE_LEVEL_11_0, // Next check D3D 11 support
	D3D_FEATURE_LEVEL_10_0, // Next, check D3D 10 support
	D3D_FEATURE_LEVEL_9_3 // Finally, check D3D 9.3 support
	};
	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsInfo;
	featureLevelsInfo.NumFeatureLevels = 5;
	featureLevelsInfo.pFeatureLevelsRequested =	featureLevels;
	md3dDevice->CheckFeatureSupport( D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsInfo, sizeof(featureLevelsInfo));


	std::wstring text = L"***Maximum Supported Feature is: ";
	std::wstring maxSupport = L"9.3";
	if (featureLevelsInfo.MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_12_1) maxSupport = L"12.1";
	if (featureLevelsInfo.MaxSupportedFeatureLevel == D3D_FEATURE_LEVEL_11_0) maxSupport = L"11.0";
	text += maxSupport;
	text += L"\n";
	OutputDebugString(text.c_str());

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_feature?redirectedfrom=MSDN

	   //Indicates a query for the level of support for basic Direct3D 12 feature options.

	D3D12_FEATURE_DATA_D3D12_OPTIONS tiers;

	if (md3dDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &tiers, sizeof(tiers))) {
		OutputDebugString(L"Failed to query the options for adapter ");
	}

	else {
		OutputDebugString(L"Options query for adapter succeeded");
	}





	//Indicates a query for the adapter's architectural details, so that your application can better optimize for certain adapter properties.
	//For example UMA: Unified Memory Architecture: a technology that allows a Graphics Processing Unit to share system memory

	D3D12_FEATURE_DATA_ARCHITECTURE1 arch;

	md3dDevice->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &arch, sizeof(arch));

	wstring gpuNode = L"Number of GPU node: ";

	gpuNode += to_wstring(md3dDevice->GetNodeCount());

	OutputDebugString(gpuNode.c_str());

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3DApp::Update(const GameTimer& gt)
{

}



void InitDirect3DApp::Draw(const GameTimer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Clear the back buffer and depth buffer.
	//mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	//Hooman Step1:
	const float color[] = { 1.0,1.0,0.0,1.0 };
	const float c = sin(mTimer.TotalTime());
	const float color2[] = { c,c,1.0,1.0 };
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), color2, 0, nullptr);

	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}
