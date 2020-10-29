#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H


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



//Device values


/////


__declspec(selectany) const enum class RenderLayer : int
{
	Opaque = 0,
	Mirrors,
	Reflected,
	Transparent,
	Shadow,
	Count
};


__declspec(selectany) std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;



// Render items divided by PSO.



static std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;











static struct RenderItem
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
static std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];
static std::vector<std::unique_ptr<RenderItem>> mAllRitems;







static void ReorderRenderItems()
{
	for (int i = 0; i < mAllRitems.size(); i++)
	{
		mAllRitems[i]->ObjCBIndex = i;
	}
}








static RenderItem* CreateRenderItem(std::string name, std::string mesh, std::string submesh, std::string mat, int layer)
{
	auto item = std::make_unique<RenderItem>();
	item->World = MathHelper::Identity4x4();
	item->TexTransform = MathHelper::Identity4x4();
	item->ObjCBIndex = mAllRitems.size();
	item->Mat = mMaterials[mat].get();
	item->Geo = mGeometries[mesh].get();
	item->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	item->IndexCount = item->Geo->DrawArgs[submesh].IndexCount;
	item->StartIndexLocation = item->Geo->DrawArgs[submesh].StartIndexLocation;
	item->BaseVertexLocation = item->Geo->DrawArgs[submesh].BaseVertexLocation;
	mRitemLayer[layer].push_back(item.get());
	mAllRitems.push_back(std::move(item));
	return mAllRitems.back().get();
	ReorderRenderItems();
}



class Task
{
public:
	virtual void Update(float totalTime, float dTime) = 0;

};
struct LightObject
{
	std::vector<Task*> tasks;
	Light light;

	void Update(float totalTime, float dTime)
	{
		for (Task* task : tasks)
		{
			task->Update(totalTime, dTime);
		};
	}

};
struct GameObject
{
	std::vector<Task*> tasks;
	std::unique_ptr<RenderItem> body;

	void Update(float totalTime, float dTime)
	{
		for (Task* task : tasks)
		{
			task->Update(totalTime, dTime);
		};
		if (tasks.size() > 0)
				body->NumFramesDirty = 3;
		
	}
};


static struct Scene
{
	bool updateInBackGround = false;

	std::vector<LightObject> directional;
	std::vector<LightObject> point;
	std::vector<LightObject> spot;

	std::vector<GameObject*> gameObjects;
	std::vector<GameObject> lights;

	GameObject* AddGameObject(GameObject* object)
	{
		gameObjects.push_back(object);

		return(object);
	}

	void AddPointLight()
	{

	}

	void AddDirectionLight()
	{

	}

	void AddSpotLight()
	{

	}

	void Update(float totalTime, float dTime)
	{
		for (auto e : gameObjects)
		{
			e->Update(totalTime, dTime);

		}
	}
	XMFLOAT3 LightCount()
	{

		return { 1,1,1 };
	}

	std::vector<Light> GetLights()
	{
		std::vector<Light> lights;
		for (auto e : directional)
			lights.push_back(e.light);
		for (auto e : point)
			lights.push_back(e.light);
		for (auto e : spot)
			lights.push_back(e.light);
		return lights;
	}
};

//Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
//Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

//
//Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
//UINT64 mCurrentFence = 0;
//
//Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
//Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;

//
//static const int SwapChainBufferCount = 2;
//int mCurrBackBuffer = 0;
//Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
//Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
//
//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
//
//D3D12_VIEWPORT mScreenViewport;
//D3D12_RECT mScissorRect;
//
//UINT mRtvDescriptorSize = 0;
//UINT mDsvDescriptorSize = 0;
//UINT mCbvSrvUavDescriptorSize = 0;
//static Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
//static Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
//static Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
//
//static Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
//static UINT64 mCurrentFence = 0;
//
//static Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
//static Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
//static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
//
//static const int SwapChainBufferCount = 2;
//static int mCurrBackBuffer = 0;
//static Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
//static Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
//
//static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
//static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
//
//static D3D12_VIEWPORT mScreenViewport;
//static D3D12_RECT mScissorRect;
//
//static UINT mRtvDescriptorSize = 0;
//static UINT mDsvDescriptorSize = 0;
//static UINT mCbvSrvUavDescriptorSize = 0;

#endif