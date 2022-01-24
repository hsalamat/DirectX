/** @file Triangle4-meshgeo.cpp
 *  @brief Shows how to draw a triangle in Direct3D 12 with color Using MeshGeometry.
 *
 *  @author Hooman Salamat
 */

#include "../../Common/d3dApp.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


class TriangleAPP : public D3DApp
{
public:
	TriangleAPP(HINSTANCE hInstance);
	TriangleAPP(const TriangleAPP& rhs) = delete;
	TriangleAPP& operator=(const TriangleAPP& rhs) = delete;
	~TriangleAPP();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;


	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildTriangleGeometry();
	void BuildPSO();

private:

	//step1: MeshGeometry will create the vertex buffer and vertex buffer view
	//ComPtr<ID3D12Resource> mVertexBuffer;
	//D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	//	std::array<Vertex, 3> vertices;
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;


	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	//The ID3DBlob interface is used to return data of arbitrary length. It's contained in D3dcompiler.lib 
	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mPSO = nullptr;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		TriangleAPP theApp(hInstance);
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

TriangleAPP::TriangleAPP(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
	mMainWndCaption = L"Simple Triangle Demo using Mesh Geometry";
}

TriangleAPP::~TriangleAPP()
{
}

bool TriangleAPP::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));


	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildTriangleGeometry();
	BuildPSO();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

void TriangleAPP::OnResize()
{
	D3DApp::OnResize();
}

void TriangleAPP::Update(const GameTimer& gt)
{

}

void TriangleAPP::Draw(const GameTimer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	//A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

	//You cannot render without a view port. 
	//viewport maps a NDC coordinate (like 0,1) to Render Target (400,0) assuming screen space 800X800 pixels.
	//also you use viewport to map to subportion of the render target (like top left rectangle of your screen)
	//also notice viewport(s), because you can have an array of view ports! & is poiting to the address of the first viewport
	mCommandList->RSSetViewports(1, &mScreenViewport);

	//Each scissor rectangle in the array corresponds to a viewport in an array of viewports
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	//Specifies the state of a resource regarding how the resource is being used.
	//D3D12_RESOURCE_STATE_PRESENT same as D3D12_RESOURCE_STATE_COMMON
	//Applications should only transition to this state for accessing a resource across different graphics engine types.
	//Specifically, a resource must be in the COMMON state before being used on a COPY queue
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));


	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);

	// Specify the buffers we are going to render to.
	//notice that "targets" not "target" in case we have few passes, filtering, etc.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, nullptr);


	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	//step3
	//mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);

	mCommandList->IASetVertexBuffers(0, 1, &mBoxGeo->VertexBufferView());
	mCommandList->IASetIndexBuffer(&mBoxGeo->IndexBufferView());   //bind it to the pipeline
	
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//step4
	//mCommandList->DrawInstanced((UINT)std::size(vertices), 1, 0, 0);
	mCommandList->DrawIndexedInstanced(mBoxGeo->DrawArgs["triangle"].IndexCount, 1, 0, 0, 0);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
		//HRESULT Present(UINT SyncInterval,UINT Flags);
	//SyncInterval: 0 - The presentation occurs immediately, there is no synchronization.
	//Flags defined by the DXGI_PRESENT constants. 0: Present a frame from each buffer (starting with the current buffer) to the output.
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}


void TriangleAPP::BuildRootSignature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  


	//// A root signature is an array of root parameters. We have no parameters yet, hence 0!
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	// create a root signature with no slot for now!
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}


void TriangleAPP::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;
	mvsByteCode = d3dUtil::CompileShader(L"Shaders\\VS1.hlsl", nullptr, "main", "vs_5_1");
	mpsByteCode = d3dUtil::CompileShader(L"Shaders\\PS1.hlsl", nullptr, "main", "ps_5_1");


	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void TriangleAPP::BuildTriangleGeometry()
{
	//step2
	std::array<Vertex, 3> vertices =
	{
				Vertex({ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Red)}),
				Vertex({ XMFLOAT3(+0.0f, +0.5f, 0.0f) , XMFLOAT4(Colors::Green) }),
				Vertex({ XMFLOAT3(+0.5f, -0.5f, 0.0f) , XMFLOAT4(Colors::Blue)}),
	};


	std::array<std::uint16_t, 3> indices =
	{

		0, 1, 2,
	};



	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = std::make_unique<MeshGeometry>();
	mBoxGeo->Name = "triangleGeo";

	//D3DCreateBlob creates a buffer given a size
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	mBoxGeo->DrawArgs["triangle"] = submesh;
}

void TriangleAPP::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	//bind the input layout to pipeline
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();

	//in old days, we use to call create vertex shader, now we hook it up to the PSO!
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;  //DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}





