#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

struct RenderItem
{
	RenderItem() = default;
	
	// World matrix of the shape that describes the object's local space
	// relative to the world space, which defines the position, orientation,

	XMFLOAT3 Position = XMFLOAT3{ 0,0,0 };
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale = XMFLOAT3{ 1,1,1 };

	// and scale of the object in the world.
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	void Update()
	{

		XMMATRIX Rotate = XMMatrixRotationY(Rotation.y);
		XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
		XMMATRIX Offset = XMMatrixTranslation(Position.x, Position.y, Position.z);
		XMMATRIX world = Rotate * scale * Offset;
		XMStoreFloat4x4(&World, world);



	}

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources;

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};
