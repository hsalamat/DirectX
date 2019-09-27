#include <iostream>
#include <unordered_map>
#include <windows.h> // for XMVerifyCPUSupport
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "MathHelper.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;



struct Material
{
	// Unique material name for lookup.
	std::string Name;

	int NumFramesDirty = 3;
	// Material constant buffer data used for shading.
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = .25f;
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};


//The above table stores the material data in system memory.In order for the GPU to
//access the material data in a shader, we need to mirror the relevant data in a constant
//buffer.Just like we did with per - object constant buffers, we add a constant buffer to each
//FrameResource that will store the constants for each material:

struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;

	// Used in texture mapping.
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

int main()
{
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	auto grass = std::make_unique<Material>();
	grass->Name = "grass";;
	grass->DiffuseAlbedo = XMFLOAT4(0.2f, 0.6f, 0.6f, 1.0f);
	grass->FresnelR0 = XMFLOAT3(0.01f, 0.01f, 0.01f);
	grass->Roughness = 0.125f;

	auto water = std::make_unique<Material>();
	water->Name = "water";
	water->DiffuseAlbedo = XMFLOAT4(0.0f, 0.2f, 0.6f, 1.0f);
	water->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	water->Roughness = 0.0f;
	mMaterials["grass"] = std::move(grass);
	mMaterials["water"] = std::move(water);

	int NumFramesDirty = 3;
	for (int frame = 0; frame < 3; frame++)
	{
		for (auto& e : mMaterials)
		{
			// Only update the cbuffer data if the constants have changed.  If the cbuffer
			// data changes, it needs to be updated for each FrameResource.
			Material* mat = e.second.get();
			if (mat->NumFramesDirty > 0)
			{
				XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

				MaterialConstants matConstants;
				matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
				matConstants.FresnelR0 = mat->FresnelR0;
				matConstants.Roughness = mat->Roughness;

				cout << "Copy " << mat->Name << " Material for frame number to GPU: " << mat->NumFramesDirty << endl;
				//currMaterialCB->CopyData(mat->MatCBIndex, matConstants);

				// Next FrameResource need to be updated too.
				mat->NumFramesDirty--;
			}
		}
	}

	system("pause");
	return 0;
}