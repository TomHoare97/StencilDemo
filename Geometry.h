#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "FrameResource.h"
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#include "Globals.h"


std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;




struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<uint32> Indices32;

	std::vector<uint16>& GetIndices16()
	{
		if (mIndices16.empty())
		{
			mIndices16.resize(Indices32.size());
			for (size_t i = 0; i < Indices32.size(); ++i)
				mIndices16[i] = static_cast<uint16>(Indices32[i]);
		}

		return mIndices16;
	}

private:
	std::vector<uint16> mIndices16;
};


void BuildFirePlane()
{
	std::array<Vertex, 10> vertices = {
		//fire
		Vertex{-1.5f, 1.5f, 0, 0, 0, 1, 0, 0.005},
		Vertex{1.5f, 1.5f, 0, 0, 0, 1,0.125, 0.005},
		Vertex{1.5f, -1.5f, 0, 0, 0, 1, 0.125, 0.125},
		Vertex{-1.5f, -1.5f, 0, 0, 0, 1, 0, 0.125},

		//log
		Vertex{-0.5f, -0.5f, 1, 0, 0, 0, 0, 0},
		Vertex{0.0f, 0.5f, 1, 0, 0, 1, 1, 0.5f},
		Vertex{0.5f, -0.5f, 1, 0, 0, 0, 0, 1},



		Vertex{-0.5f, -0.5f, -1, 0, 0, 0, 0, 0},
		Vertex{0.0f, 0.5f, -1, 0, 0, -1,1, 0.5f},
		Vertex{0.5f, -0.5f, -1, 0, 0, 0, 0, 1}

	};
	std::array<std::int16_t, 30> indices =
	{//fire
		0, 1, 2,
		0, 2, 3,
		//log
		//ff
			2, 1, 0,
			//bf
				3, 4 ,5,

				//rf	
					1, 2, 4,

					4, 2, 5,
					//bf
						3,2 ,0,
						5, 2, 3,
						//rf
							3, 0, 1,
							1, 4, 3


	};
	SubmeshGeometry fire;
	fire.IndexCount = 6;
	fire.StartIndexLocation = 0;
	fire.BaseVertexLocation = 0;

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);


	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "fireGeo";


	SubmeshGeometry log;
	log.IndexCount = 24;
	log.StartIndexLocation = 6;
	log.BaseVertexLocation = 4;




	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["fire"] = fire;
	geo->DrawArgs["log"] = log;

	mGeometries[geo->Name] = std::move(geo);

}