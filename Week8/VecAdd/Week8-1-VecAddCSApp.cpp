//***************************************************************************************
// VecAddCSApp.cpp how to add two sets of 32 vectors using 32 threads in the compute shader
//***************************************************************************************

#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")


//step1
struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};


class VecAddApp : public D3DApp
{
public:
    VecAddApp(HINSTANCE hInstance);
    VecAddApp(const VecAddApp& rhs) = delete;
    VecAddApp& operator=(const VecAddApp& rhs) = delete;
    ~VecAddApp();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt)override;
    virtual void Draw(const GameTimer& gt)override;

	//step2
	void DoComputeWork();

	void BuildBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSOs();

private:

    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	//step3: we have 2 sets of 32 vectors that we want to add. Same operation for all vectors!
	const int NumDataElements = 32;

	ComPtr<ID3D12Resource> mInputBufferA = nullptr;
	ComPtr<ID3D12Resource> mInputUploadBufferA = nullptr;
	ComPtr<ID3D12Resource> mInputBufferB = nullptr;
	ComPtr<ID3D12Resource> mInputUploadBufferB = nullptr;
	ComPtr<ID3D12Resource> mOutputBuffer = nullptr;
	ComPtr<ID3D12Resource> mReadBackBuffer = nullptr;

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
        VecAddApp theApp(hInstance);
        if(!theApp.Initialize())
            return 0;
		//step 3: we don't need to have a game loop
       // return theApp.Run();
		return 0;
    }
    catch(DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

VecAddApp::VecAddApp(HINSTANCE hInstance)
    : D3DApp(hInstance)
{
}

VecAddApp::~VecAddApp()
{
    if(md3dDevice != nullptr)
        FlushCommandQueue();
}

bool VecAddApp::Initialize()
{
    if(!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	//step4
	BuildBuffers();
    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildPSOs();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

	DoComputeWork();

    return true;
}
 
void VecAddApp::OnResize()
{
}

void VecAddApp::Update(const GameTimer& gt)
{
}

void VecAddApp::Draw(const GameTimer& gt)
{
}

 //step 5
void VecAddApp::DoComputeWork()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSOs["vecAdd"].Get()));

	mCommandList->SetComputeRootSignature(mRootSignature.Get());

	mCommandList->SetComputeRootShaderResourceView(0, mInputBufferA->GetGPUVirtualAddress());
	mCommandList->SetComputeRootShaderResourceView(1, mInputBufferB->GetGPUVirtualAddress());
	mCommandList->SetComputeRootUnorderedAccessView(2, mOutputBuffer->GetGPUVirtualAddress());
 
	mCommandList->Dispatch(1, 1, 1);

	// Schedule to copy the data to the default buffer to the readback buffer.

	//D3D12_RESOURCE_STATE_COPY_SOURCE: The resource is used as the source in a copy operation. 
	//D3D12_RESOURCE_STATE_COPY_DEST: The resource is used as the destination in a copy operation.

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mOutputBuffer.Get(), 
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));

	//We need to read the data back from the GPU after computer shader does its calculation
	//mCommandList->CopyResource(Destination: CPUData, Source: GPUData);
	mCommandList->CopyResource(mReadBackBuffer.Get(), mOutputBuffer.Get());

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mOutputBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));


	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait for the work to finish.
	FlushCommandQueue();

	// Map the data so we can read it on CPU.
	Data* mappedData = nullptr;
	ThrowIfFailed(mReadBackBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));

	std::ofstream fout("results.txt");

	for(int i = 0; i < NumDataElements; ++i)
	{
		fout << "(" << mappedData[i].v1.x << ", " << mappedData[i].v1.y << ", " << mappedData[i].v1.z <<
			", " << mappedData[i].v2.x << ", " << mappedData[i].v2.y << ")" << std::endl;
	}

	mReadBackBuffer->Unmap(0, nullptr);
}

void VecAddApp::BuildBuffers()
{
	// Generate some data.
	std::vector<Data> dataA(NumDataElements);
	std::vector<Data> dataB(NumDataElements);
	for(int i = 0; i < NumDataElements; ++i)
	{
		dataA[i].v1 = XMFLOAT3((float)i, (float)i, (float)i);
		dataA[i].v2 = XMFLOAT2((float)i, 0.0f);

		dataB[i].v1 = XMFLOAT3(-(float)i, (float)i, 0.0f);
		dataB[i].v2 = XMFLOAT2(0.0f, -(float)i);
	}

	UINT64 byteSize = dataA.size()*sizeof(Data);

	// Create some buffers to be used as SRVs.
	mInputBufferA = d3dUtil::CreateDefaultBuffer(
		md3dDevice.Get(),
		mCommandList.Get(),
		dataA.data(),
		byteSize,
		mInputUploadBufferA);

	mInputBufferB = d3dUtil::CreateDefaultBuffer(
		md3dDevice.Get(),
		mCommandList.Get(),
		dataB.data(),
		byteSize,
		mInputUploadBufferB);

	// Create the buffer that will be a UAV.
	ThrowIfFailed(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&mOutputBuffer)));
	
	//Specifies a heap used for reading back. This heap type has CPU access optimized for reading data back from the GPU, 
	//but does not experience the maximum amount of bandwidth for the GPU. This heap type is best for GPU-write-once, 
	//CPU-readable data. The CPU cache behavior is write-back, which is conducive for multiple sub-cache-line CPU reads.
	//Resources in this heap must be created with D3D12_RESOURCE_STATE_COPY_DEST, and cannot be changed away from this
	ThrowIfFailed(md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&mReadBackBuffer)));
}

void VecAddApp::BuildRootSignature()
{
    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Perfomance TIP: Order from most frequent to least frequent.
	slotRootParameter[0].InitAsShaderResourceView(0);
    slotRootParameter[1].InitAsShaderResourceView(1);
    slotRootParameter[2].InitAsUnorderedAccessView(0);

    // A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter,
		0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_NONE);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if(errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}


void VecAddApp::BuildShadersAndInputLayout()
{
	mShaders["vecAddCS"] = d3dUtil::CompileShader(L"Shaders\\VecAdd.hlsl", nullptr, "CS", "cs_5_1");
}

void VecAddApp::BuildPSOs()
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc = {};
	computePsoDesc.pRootSignature = mRootSignature.Get();
	computePsoDesc.CS =
	{
		reinterpret_cast<BYTE*>(mShaders["vecAddCS"]->GetBufferPointer()),
		mShaders["vecAddCS"]->GetBufferSize()
	};
	computePsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateComputePipelineState(&computePsoDesc, IID_PPV_ARGS(&mPSOs["vecAdd"])));
}





